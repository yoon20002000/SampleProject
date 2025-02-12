#include "UI/TPSActionMenuWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
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

	UseButton->OnClicked.AddDynamic(this,&ThisClass::OnClickedUse);
	Drop1Button->OnClicked.AddDynamic(this,&ThisClass::OnClickedDrop1);
	DropAllButton->OnClicked.AddDynamic(this,&ThisClass::OnClickedDropAll);
	
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	UCanvasPanelSlot* BoxPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ButtonsScrollBox);
	BoxPanelSlot->SetPosition(MousePosition);
}

void UTPSActionMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}

void UTPSActionMenuWidget::OnClickedUse()
{
	UE_LOG(LogTemp, Log, TEXT("Use Item : %s"), *InventoryComp->GetInventorySlotOrNullptr(InteractionTargetIndex)->ItemName.ToString());
}

void UTPSActionMenuWidget::OnClickedDrop1()
{
	InventoryComp->RemoveItem(InteractionTargetIndex, false, false);
}

void UTPSActionMenuWidget::OnClickedDropAll()
{
	InventoryComp->RemoveItem(InteractionTargetIndex, true, false);
}
