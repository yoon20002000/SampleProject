#include "Components/TPSItemDataComponent.h"

#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "Data/GameDataAsset.h"

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

void UTPSItemDataComponent::SetItemData(const FName& ItemName, const int32 InQuantity)
{
	ItemDataHandle.RowName = ItemName;
	Quantity = InQuantity; 
}
