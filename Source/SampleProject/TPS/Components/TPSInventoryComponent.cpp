#include "Components/TPSInventoryComponent.h"

#include "TPSGameManager.h"
#include "TPSHelper.h"
#include "TPSItemDataComponent.h"
#include "TPSSystemManager.h"
#include "Actor/TPSInteractionActorBase.h"
#include "Actor/TPSItemActor.h"
#include "Game/TPSGameInstance.h"
#include "System/TPSCollisionChannels.h"


FItem::FItem() : Name(NAME_None), Description(FText::GetEmpty()), Thumbnail(nullptr), ItemClass(nullptr), StackSize(1)
{
}

bool FInventorySlot::IsEmpty() const
{
	return ItemName == NAME_None;
}

void FInventorySlot::SetEmpty()
{
	ItemName = NAME_None;
	ItemQuantity = 0;
}

void FInventorySlot::SetSlot(const FName& InItemName, const int32 Quantity)
{
	ItemName = InItemName;
	ItemQuantity = Quantity;
}

UTPSInventoryComponent::UTPSInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


FInventorySlot* UTPSInventoryComponent::FindSameItemAddableSlot(const FName& ItemName)
{
	for (FInventorySlot& Slot: Inventory)
	{
		if (Slot.ItemName.IsEqual(ItemName) == true && Slot.ItemQuantity < GetMaxStackSize(ItemName))
		{
			return &Slot;
		}
	}
	return nullptr;
}

void UTPSInventoryComponent::ConsumeItem(UTPSAbilitySystemComponent* InstigatorASC, const int32 InventorySlotIndex)
{
	if (InventorySlotIndex < 0 || InventorySlotIndex >= Inventory.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("Slot Index is out of range! %d"), InventorySlotIndex);
		return;
	}

	FInventorySlot& TargetItem = Inventory[InventorySlotIndex];
	
	if (FItem* ItemData = GetItemDataOrNullptr(TargetItem.ItemName))
	{
		FGameplayEffectContextHandle EffectContext = InstigatorASC->MakeEffectContext();

		UClass* GE = ItemData->ApplyEffectClass.LoadSynchronous();
		
		const UGameplayEffect* Effect = GE->GetDefaultObject<UGameplayEffect>();
	
		InstigatorASC->ApplyGameplayEffectToSelf(Effect,1,EffectContext);
		
		RemoveItem(InventorySlotIndex, false, true);
	}
}

const TArray<FInventorySlot>& UTPSInventoryComponent::GetInventorySlots()
{
	return Inventory;
}

void UTPSInventoryComponent::TransferSlots(const int32 SourceIndex, UTPSInventoryComponent* SourceInventoryComp,
                                           const int32 DestinationIndex)
{

	if (SourceIndex < 0 || SourceIndex >= SourceInventoryComp->GetInventorySlotSize() ||
		DestinationIndex < 0 || DestinationIndex >= GetInventorySlotSize())
	{
		UE_LOG(LogTemp, Error, TEXT("Index Range Error!! SourceIndex : %d, DestinationIndex : %d"), SourceIndex,
		       DestinationIndex);
		return;
	}
	
	FInventorySlot& SourceInventorySlot = SourceInventoryComp->Inventory[SourceIndex];
	FInventorySlot& DestinationInventorySlot = Inventory[DestinationIndex];
	if (SourceInventorySlot.ItemName == DestinationInventorySlot.ItemName)
	{
		if (FItem* ItemData= GetItemDataOrNullptr(SourceInventorySlot.ItemName))
		{
			const int32 MaxStack = ItemData->StackSize;
			if (DestinationInventorySlot.ItemQuantity == MaxStack)
			{
				SwapInventorySlots(SourceInventorySlot ,DestinationInventorySlot);
			}
			else
			{
				AddQuantityClampMaxStack(SourceInventorySlot, DestinationInventorySlot, MaxStack);
			}
		}
	}
	else
	{
		SwapInventorySlots(SourceInventorySlot, DestinationInventorySlot);
	}
	
	OnInventoryUpdatedDelegate.Broadcast();
	SourceInventoryComp->OnInventoryUpdatedDelegate.Broadcast();
}

int32 UTPSInventoryComponent::GetInventorySlotSize() const
{
	return InventorySlotMaxSize;
}

const FInventorySlot* UTPSInventoryComponent::GetInventorySlotOrNullptr(const int32 SlotIndex) const
{
	return &Inventory[SlotIndex];
}

FItem* UTPSInventoryComponent::GetItemDataOrNullptr(const FName& ItemName) const
{

	FItem* ItemData = UTPSSystemManager::Get()->GetGameManager()->GetItemDataOrNullptr(ItemName);
	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemData is null Item Name is %s"), *ItemName.ToString());
		return nullptr;
	}
	
	return ItemData;
}

bool UTPSInventoryComponent::HaveEnoughItems(const FName& ItemName, const int32 Quantity) const
{
	for (int Index = 0; Index < InventorySlotMaxSize; ++Index)
	{
		if (Inventory[Index].ItemName.IsEqual(ItemName) &&  Inventory[Index].ItemQuantity> Quantity)
		{
			return true;
		}
	}
	return false;
}

void UTPSInventoryComponent::SetMaxSlotSize(int32 MaxStackSize)
{
	InventorySlotMaxSize = MaxStackSize;
	if (Inventory.Num() < InventorySlotMaxSize)
	{
		int32 CurSize = Inventory.Num();
		Inventory.Reserve(InventorySlotMaxSize);
		for (int32 AddCount = 0; AddCount < MaxStackSize - CurSize; ++AddCount)
		{
			Inventory.Add(FInventorySlot());
		}
	}
}

int32 UTPSInventoryComponent::GetMaxStackSize() const
{
	return InventorySlotMaxSize;
}

void UTPSInventoryComponent::SaveInventory()
{
	// To do 임시 코드 해결 방안 필요
	if (GetOwner()->IsA(ATPSPlayer::StaticClass()) == false)
	{
		return;
	}
	if (UTPSGameInstance* GI = Cast<UTPSGameInstance>(GetWorld()->GetGameInstance()))
	{
		UE_LOG(LogTemp, Log, TEXT("Saved"));
		GI->SaveInventoryData(Inventory);
	}
}

void UTPSInventoryComponent::LoadInventory()
{
	if (UTPSGameInstance* GI = Cast<UTPSGameInstance>(GetWorld()->GetGameInstance()))
	{
		Inventory = GI->GetInventoryData();
	}
}

void UTPSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Inventory.Reserve(InventorySlotMaxSize);
	for (int i =  Inventory.Num(); i < InventorySlotMaxSize; ++i)
	{
		Inventory.Add(FInventorySlot());
	}
	
	OnInventoryUpdatedDelegate.AddUObject(this, &ThisClass::SaveInventory);
}

void UTPSInventoryComponent::AddNewItemToInventory(const FName& ItemName, const int32 Quantity)
{
	bool bIsDirty = false;
	for (int32 Index = 0; Index < InventorySlotMaxSize; ++Index)
	{
		if (Inventory[Index].ItemQuantity <= 0)
		{
			Inventory[Index].ItemName = ItemName;
			Inventory[Index].ItemQuantity = Quantity;
			bIsDirty = true;
			break;
		}
	}
	if (bIsDirty == true)
	{
		OnInventoryUpdatedDelegate.Broadcast();	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Add Error! Not Enough Slot!!"));	
	}
}

void UTPSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceItem();
}

void UTPSInventoryComponent::AddItemQuantity(const int32 Quantity, FInventorySlot* Slot)
{
	Slot->ItemQuantity += Quantity;
	OnInventoryUpdatedDelegate.Broadcast();
}

void UTPSInventoryComponent::AddQuantityClampMaxStack(FInventorySlot& SourceInventorySlot, FInventorySlot& DestinationInventorySlot, const int32 MaxStack)
{
	const int32 TotalQuantity = SourceInventorySlot.ItemQuantity + DestinationInventorySlot.ItemQuantity;
	DestinationInventorySlot.ItemQuantity = FMath::Clamp(TotalQuantity, 0, MaxStack);
	SourceInventorySlot.ItemQuantity = TotalQuantity - MaxStack;
	if (SourceInventorySlot.ItemQuantity <= 0)
	{
		SourceInventorySlot.SetEmpty();
	}
}

void UTPSInventoryComponent::AddItemToSlot(const int32 SlotIndex, const FName& ItemName, const int32 Quantity)
{
	FInventorySlot& TargetSlot = Inventory[SlotIndex];
	
	if (TargetSlot.ItemName == ItemName)
	{
		if ( GetMaxStackSize(ItemName) < TargetSlot.ItemQuantity + Quantity)
		{
			AddItemQuantity(Quantity, &TargetSlot);
		}
		else
		{
			// already full stack
			UE_LOG(LogTemp, Warning, TEXT("Already full stack. Actor Name : %s, Slot Number : %d"),*GetOwner()->GetName(), SlotIndex);
			AddNewItemToInventory(ItemName,Quantity);
		}
	}
	else if (TargetSlot.IsEmpty() == true)
	{
		TargetSlot.SetSlot(ItemName, Quantity);
	}
	else
	{
		// other item is already exist
		UE_LOG(LogTemp, Warning, TEXT("Other item is already exist. Actor Name : %s, Slot Number : %d"),*GetOwner()->GetName(), SlotIndex);
		AddNewItemToInventory(ItemName,Quantity);
	}
}

void UTPSInventoryComponent::AddItemToInventory(const FName& ItemName, const int32 Quantity)
{
	FInventorySlot* Slot = FindSameItemAddableSlot(ItemName);
	
	int32 MaxStackSize = GetMaxStackSize(ItemName);
	if (MaxStackSize < Quantity)
	{
		UE_LOG(LogTemp, Error, TEXT("Item Actor Quantity is error!!! Name : %s, Quantity : %d"), *ItemName.ToString(),
		       Quantity);
	}
	
	if (Slot != nullptr)
	{
		const int32 TotalQuantity = Quantity + Slot->ItemQuantity;
		if (TotalQuantity > MaxStackSize)
		{
			AddNewItemToInventory(ItemName, Quantity);
		}
		else
		{
			AddItemQuantity(Quantity, Slot);	
		}
	}
	else
	{
		AddNewItemToInventory(ItemName, Quantity);
	}
}

int32 UTPSInventoryComponent::GetMaxStackSize(const FName& ItemName) const
{
	FItem* ItemData = GetItemDataOrNullptr(ItemName);
	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Item Data is nullptr!! Check Item Data Table! %s"), *ItemName.ToString());
		return -1;
	}

	return ItemData->StackSize;
}

void UTPSInventoryComponent::RemoveItem(const int32 Index, const bool bRemoveAll, const bool bIsConsumed)
{
	if (Index < 0 || Index >= Inventory.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("Index i not in range off array!!"));
		return;
	}
	
	FInventorySlot& TargetSlot = Inventory[Index];

	if (TargetSlot.ItemQuantity == 1 || bRemoveAll == true)
	{
		if (bIsConsumed == false)
		{
			DropItem(TargetSlot.ItemName,TargetSlot.ItemQuantity);
		}
		TargetSlot.SetEmpty();
	}
	else
	{
		if (bIsConsumed == false)
		{
			DropItem(TargetSlot.ItemName, 1);
		}
		TargetSlot.ItemQuantity -= 1;
	}

	OnInventoryUpdatedDelegate.Broadcast();
}

void UTPSInventoryComponent::DropItem(const FName& ItemName, const int32 Quantity)
{
	AActor* OwnerActor = GetOwner();
	
	FVector SpawnLocation = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * DropForwardDistance;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	if (ATPSItemActor* DropItemActor = UTPSSystemManager::Get()->GetGameManager()->SpawnItemActor<ATPSItemActor>(ItemName, SpawnLocation, SpawnRotation))
	{
		DropItemActor->SetItemData(ItemName, Quantity);
	}
}

void UTPSInventoryComponent::TraceItem()
{
	if (AActor* OwnerActor = GetOwner())
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		OwnerActor->GetActorEyesViewPoint(OUT EyeLocation, OUT EyeRotation);
		FVector TraceStart = EyeLocation;

		APlayerCameraManager* CameraManager = TPSHelper::GetPlayerCameraManager();
		FVector CamForwardVector = CameraManager->GetActorForwardVector();
		FVector TraceEnd = TraceStart + CamForwardVector * SweepDistance;

		TArray<FHitResult> Hits;
		FCollisionQueryParams CollisionQueryParameters(TEXT("Interact Item"), true, OwnerActor);
		TArray<AActor*> AttachedActors;
		OwnerActor->GetAttachedActors(OUT AttachedActors);
		CollisionQueryParameters.AddIgnoredActors(AttachedActors);
		GetWorld()->LineTraceMultiByChannel(Hits, TraceStart, TraceEnd,TPS_TraceChannel_Interaction,
		                                    CollisionQueryParameters);

		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue);

		AActor* FirstHitActor = GetFirstHitItemInteraction(Hits);

		if (FirstHitActor == nullptr)
		{
			FCollisionShape SphereShape = FCollisionShape::MakeSphere(SweepSphereRadius);
			FVector AdditionalTraceStart = TraceStart + CameraManager->GetActorForwardVector() * SweepDistance;
			FVector AdditionalTraceEnd = AdditionalTraceStart;
			GetWorld()->SweepMultiByChannel(Hits, AdditionalTraceStart, AdditionalTraceEnd, FQuat::Identity,
			                                TPS_TraceChannel_Interaction,
			                                SphereShape, CollisionQueryParameters);

			DrawDebugSphere(GetWorld(), AdditionalTraceStart, SweepSphereRadius, 12, FColor::Yellow);

			FirstHitActor = GetFirstHitItemInteraction(Hits);
		}


		if (FirstHitActor == nullptr || FirstHitActor != CurHitActor)
		{
			CurHitActor = FirstHitActor;
			if (CurHitActor != nullptr)
			{
				// ITPSInteractionInterface* InteractionInterface = Cast<ITPSInteractionInterface>(CurHitActor);
				// InteractionInterface->LookAtInteractionActor();
				// InteractionInterface->Interaction();
			}
		}
	}
}

AActor* UTPSInventoryComponent::GetFirstHitItemInteraction(const TArray<FHitResult>& Hits) const
{
	if (Hits.Num() > 0)
	{
		for (const FHitResult& Hit : Hits)
		{
			if (Hit.GetActor()->GetComponentByClass<UTPSItemDataComponent>() != nullptr)
			{
				return Hit.GetActor();
			}
		}
	}
	return nullptr;
}
void UTPSInventoryComponent::SwapInventorySlots(FInventorySlot& A, FInventorySlot& B)
{
	FInventorySlot Temp = A;
	A = B;
	B = Temp;
}
