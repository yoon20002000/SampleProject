#include "Actor/TPSItemActor.h"

#include "Components/TPSItemDataComponent.h"

ATPSItemActor::ATPSItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	ItemDataComp = CreateDefaultSubobject<UTPSItemDataComponent>(TEXT("Item Data Comp"));
}

void ATPSItemActor::Interaction(AActor* InteractionActor)
{
	if (UTPSInventoryComponent* InventoryComponent = InteractionActor->GetComponentByClass<UTPSInventoryComponent>())
	{
		ItemDataComp->InteractionItem(InventoryComponent);
	}
}

UTPSItemDataComponent* ATPSItemActor::GetItemDataComponent()
{
	return ItemDataComp;
}

void ATPSItemActor::SetItemData(const FName& ItemName, const int32 Quantity)
{
	if (ItemDataComp != nullptr)
	{
		ItemDataComp->SetItemData(ItemName, Quantity);	
	}
}
