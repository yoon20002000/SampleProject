#include "Components/TPSItemDataComponent.h"

UTPSItemDataComponent::UTPSItemDataComponent() : Quantity(1)
{
	PrimaryComponentTick.bCanEverTick = false;
}
FItem* UTPSItemDataComponent::GetItemData()
{
	return ItemDataHandle.GetRow<FItem>(TEXT("Item Data"));
}

void UTPSItemDataComponent::InteractionItem(UTPSInventoryComponent* InventoryComponent)
{
	FItem* CurItem = GetItemData();
	InventoryComponent->AddItemToInventory(CurItem->Name, Quantity);
	GetOwner()->Destroy(true);
}
