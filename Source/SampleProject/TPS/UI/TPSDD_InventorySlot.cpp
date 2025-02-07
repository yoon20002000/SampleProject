#include "UI/TPSDD_InventorySlot.h"
#include "Components/TPSInventoryComponent.h"

void UTPSDD_InventorySlot::Init(UTPSInventoryComponent* InInventoryComponent, const int32 InContentIndex)
{
	InventoryComponent = InInventoryComponent;
	ContentIndex = InContentIndex;
}
