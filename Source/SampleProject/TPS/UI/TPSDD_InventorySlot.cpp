#include "UI/TPSDD_InventorySlot.h"
#include "Components/TPSInventoryComponent.h"

void UTPSDD_InventorySlot::Init(UTPSInventoryComponent* InInventoryComponent, const int32 InContentIndex)
{
	InventoryComp = InInventoryComponent;
	ContentIndex = InContentIndex;
}

int32 UTPSDD_InventorySlot::GetContentIndex() const
{
	return ContentIndex;
}

UTPSInventoryComponent* UTPSDD_InventorySlot::GetInventoryComponent() const
{
	return InventoryComp.Get();
}
