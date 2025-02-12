#include "UI/TPSActionMenuWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TPSInventoryComponent.h"
#include "Components/VerticalBox.h"

void UTPSActionMenuWidget::Init(UTPSInventoryComponent* InInventoryComp, const int32 InTargetIndex)
{
	InventoryComp = InInventoryComp;
	InteractionTargetIndex = InTargetIndex;
}

void UTPSActionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	UCanvasPanelSlot* BoxPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ButtonsScrollBox);
	BoxPanelSlot->SetPosition(MousePosition);
}

void UTPSActionMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}
