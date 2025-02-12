#include "Components/TPSInventoryComponent.h"

#include "TPSGameManager.h"
#include "TPSHelper.h"
#include "TPSItemDataComponent.h"
#include "TPSSystemManager.h"
#include "System/TPSCollisionChannels.h"


FItem::FItem() : Name(NAME_None), Description(FText::GetEmpty()), Thumbnail(nullptr), ItemClass(nullptr), StackSize(1)
{
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

const TArray<FInventorySlot>& UTPSInventoryComponent::GetInventorySlots()
{
	return Inventory;
}

void UTPSInventoryComponent::InteractionWithCurHitItem()
{
	if (CurHitActor != nullptr)
	{
		if (UTPSItemDataComponent* ItemDataComp = CurHitActor->GetComponentByClass<UTPSItemDataComponent>())
		{
			// ItemDataComp->Interaction(CurHitActor.Get());
		}
	}
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
	
	TArray<FInventorySlot>& SourceInventory = SourceInventoryComp->Inventory;
	if (SourceInventory[SourceIndex].ItemName == Inventory[DestinationIndex].ItemName)
	{
		if (FItem* ItemData= UTPSSystemManager::Get()->GetGameManager()->GetItem(SourceInventory[SourceIndex].ItemName))
		{
			int32 MaxStack = ItemData->StackSize;
			if (Inventory[DestinationIndex].ItemQuantity == MaxStack)
			{
				SwapInventorySlots(SourceInventory[SourceIndex], Inventory[DestinationIndex]);
			}
			else
			{
				const int32 TotalQuantity = SourceInventory[SourceIndex].ItemQuantity + Inventory[DestinationIndex].ItemQuantity;
				Inventory[DestinationIndex].ItemQuantity = FMath::Clamp(TotalQuantity, 0, MaxStack);
				SourceInventory[SourceIndex].ItemQuantity = TotalQuantity - MaxStack;	
			}
		}
	}
	else
	{
		SwapInventorySlots(SourceInventory[SourceIndex], Inventory[DestinationIndex]);
	}
	
	OnInventoryUpdatedDelegate.Broadcast();
	SourceInventoryComp->OnInventoryUpdatedDelegate.Broadcast();
}

int32 UTPSInventoryComponent::GetInventorySlotSize() const
{
	return InventorySlotMaxSize;
}

void UTPSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// 이후 Save, Load 추가 시 기능 추가 필요
	Inventory.Reserve(InventorySlotMaxSize);
	for (int i =  0; i < InventorySlotMaxSize; ++i)
	{
		Inventory.Add(FInventorySlot());
	}
}

void UTPSInventoryComponent::AddNewItemToInventory(const FName& ItemName, const int32 Quantity)
{
	for (FInventorySlot& Slot: Inventory)
	{
		if (Slot.ItemQuantity <= 0)
		{
			Slot.ItemName = ItemName;
			Slot.ItemQuantity = Quantity;
			break;
		}
	}
}

void UTPSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceItem();
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
			Slot->ItemQuantity += Quantity;	
		}
	}
	else
	{
		AddNewItemToInventory(ItemName, Quantity);
	}
}

int32 UTPSInventoryComponent::GetMaxStackSize(const FName& ItemName) const
{
	FItem* ItemData = UTPSSystemManager::Get()->GetGameManager()->GetItem(ItemName);
	if (ItemData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Item Data is nullptr!! Check Item Data Table! %s"), *ItemName.ToString());
		return -1;
	}

	return ItemData->StackSize;
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
