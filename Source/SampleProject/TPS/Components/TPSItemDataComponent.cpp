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
	// inventorycomp에 해당 아이템을 추가 할 수 있는지 확인 후 additemtoinventory 하게 해야 됨.
	InventoryComponent->AddItemToInventory(CurItem->Name, Quantity);
	GetOwner()->Destroy(true);
}
