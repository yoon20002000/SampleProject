#include "System/SaveLoad/TPSSaveGame.h"

#include "Components/TPSInventoryComponent.h"

void UTPSSaveGame::SetInventoryData(const TArray<FInventorySlot>& InventoryData)
{
	Inventory.Empty();

	Inventory.Reserve(InventoryData.Num());
	Inventory = InventoryData;
	// for (int32 Index = 0; Index < InventoryData.Num(); ++Index)
	// {
	// 	Inventory.Emplace(InventoryData[Index]);
	// }
}

const TArray<FInventorySlot>& UTPSSaveGame::GetInventoryData() const
{
	return Inventory;
}
