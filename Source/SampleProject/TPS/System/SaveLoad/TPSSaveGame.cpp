#include "System/SaveLoad/TPSSaveGame.h"

#include "Components/TPSInventoryComponent.h"

void UTPSSaveGame::SetInventoryData(const TArray<FInventorySlot>& InventoryData)
{
	Inventory.Empty();
	Inventory = InventoryData;
}

const TArray<FInventorySlot>& UTPSSaveGame::GetInventoryData() const
{
	return Inventory;
}
