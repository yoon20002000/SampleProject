#include "UI/TPSActionMenuWidget.h"
#include "Components/TPSInventoryComponent.h"

void UTPSActionMenuWidget::Init(UTPSInventoryComponent* InInventoryComp, const int32 InTargetIndex)
{
	InventoryComp = InInventoryComp;
	InteractionTargetIndex = InTargetIndex;
}
