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


FInventorySlot* UTPSInventoryComponent::FindAddSlot(const FName& ItemName)
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

void UTPSInventoryComponent::CreateNewSlotAndAddToInventory(const FName& ItemName, const int32 Quantity)
{
	FInventorySlot NewSlot(ItemName, Quantity);
	Inventory.Add(NewSlot);
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
			ItemDataComp->Interaction(this);
		}
	}
}

void UTPSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Inventory.Reserve(InventoryMaxSize);
}

void UTPSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceItem();
}

void UTPSInventoryComponent::AddItemToInventory(const FName& ItemName, const int32 Quantity)
{
	FInventorySlot* Slot = FindAddSlot(ItemName);

	
	int32 MaxStackSize = GetMaxStackSize(ItemName);
	
	UE_LOG(LogTemp, Log, TEXT("Item Max Stack Size : %d"), MaxStackSize);
	// 없는 경우 빈 슬롯 찾아서 추가
	// 있는데 널널 한 경우 그냥 수 추가
	// 있긴한데 공간 좀 부족 한 경우 하나 맥스 처리 하고 나머지를 빈 슬롯 찾아서 추가

	if (Slot != nullptr)
	{
		Slot->ItemQuantity += Quantity;
		// if (Slot->ItemQuantity + Quantity > MaxStackSize)
		// {
		// 	int32 LeftSize =  Slot->ItemQuantity + Quantity - MaxStackSize;
		// 	Slot->ItemQuantity = MaxStackSize;
		// 	// 빈 슬롯 찾아서 left size 만틈 추가
		// }
		// else
		// {
		// 	Slot->ItemQuantity += Quantity;
		// }
	}
	else
	{
		// quantity가 max 보다 클 경우를 대비 해for문으로 작성 해야 됨.
		CreateNewSlotAndAddToInventory(ItemName, Quantity);
		// 빈 슬롯 추가
	}
	
	// if (Slot != nullptr && Slot->ItemQuantity )
	// {
	// 	Slot->ItemQuantity += Quantity;
	// }
	// else
	// {
	// 	
	// }
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
		GetWorld()->LineTraceMultiByChannel(Hits, TraceStart, TraceEnd,TPS_TraceChannel_ItemInteraction,
		                                    CollisionQueryParameters);

		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue);

		AActor* FirstHitActor = GetFirstHitItemInteraction(Hits);

		if (FirstHitActor == nullptr)
		{
			FCollisionShape SphereShape = FCollisionShape::MakeSphere(SweepSphereRadius);
			FVector AdditionalTraceStart = TraceStart + CameraManager->GetActorForwardVector() * SweepDistance;
			FVector AdditionalTraceEnd = AdditionalTraceStart;
			GetWorld()->SweepMultiByChannel(Hits, AdditionalTraceStart, AdditionalTraceEnd, FQuat::Identity,
			                                TPS_TraceChannel_ItemInteraction,
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
