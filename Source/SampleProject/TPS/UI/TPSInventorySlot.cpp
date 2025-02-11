#include "UI/TPSInventorySlot.h"

#include "CommonNumericTextBlock.h"
#include "TPSDD_InventorySlot.h"
#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TPSInventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/TPSDrawPreview.h"

bool UTPSInventorySlot::Initialize()
{
	return Super::Initialize();
}

void UTPSInventorySlot::Init(UTPSInventoryComponent* InInventoryComp, const int32 InSlotIndex, const FName& InItemName, const int32 ItemQuantity)
{
	InventoryComp = InInventoryComp;
	InventorySlotIndex = InSlotIndex;
	ItemName = InItemName;
	UpdateInventorySlot(InItemName, ItemQuantity);
}

void UTPSInventorySlot::UpdateInventorySlot(const FName& InItemName, const int32 Quantity)
{
	if (InItemName == NAME_None)
	{
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
		QuantityBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (const FItem* Item = UTPSSystemManager::Get()->GetGameManager()->GetItem(InItemName))
	{
		if (Item->Thumbnail != nullptr && Quantity > 0)
		{
			IconImage->SetBrushFromTexture(Item->Thumbnail);
			IconImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			IconImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	QuantityBox->SetVisibility(Quantity > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	QuantityText->SetCurrentValue(Quantity);
}

FReply UTPSInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	
	if (InventoryComp == nullptr || ItemName == NAME_None)
	{
		return FReply::Unhandled();
	}
	
	// To do이건 enhanced input으로 바꿔야 함.
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
	}

	return FReply::Unhandled();
}

void UTPSInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (UTPSDrawPreview* NewDragPreviewWidget = CreateWidget<UTPSDrawPreview>(this,DragPreviewClass.LoadSynchronous()))
	{
		NewDragPreviewWidget->SetPreview(ItemName);
		if (UTPSDD_InventorySlot* DD_InventorySlot =
			Cast<UTPSDD_InventorySlot>(UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOperationClass.LoadSynchronous())))
		{
			DD_InventorySlot->DefaultDragVisual = NewDragPreviewWidget;
			DD_InventorySlot->Init(InventoryComp.Get(),InventorySlotIndex);
			OutOperation = DD_InventorySlot;
		}
	}
}

bool UTPSInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UTPSDD_InventorySlot* DD_InventorySlot = Cast<UTPSDD_InventorySlot>(InOperation))
	{
		if (InventoryComp == nullptr || DD_InventorySlot->GetInventoryComponent() == nullptr)
		{
			return false;
		}
		const int32& StartSlotIndex = DD_InventorySlot->GetContentIndex();
		if (InventorySlotIndex != StartSlotIndex ||
		InventoryComp != DD_InventorySlot->GetInventoryComponent())
		{
			InventoryComp->TransferSlots(StartSlotIndex, DD_InventorySlot->GetInventoryComponent(), InventorySlotIndex);
			return true;
		}
	}
		
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
