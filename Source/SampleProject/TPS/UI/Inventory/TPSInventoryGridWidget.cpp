#include "UI/Inventory/TPSInventoryGridWidget.h"

#include "Components/TPSInventoryComponent.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "UI/TPSInventorySlot.h"

void UTPSInventoryGridWidget::Init(UTPSInventoryComponent* InInventoryComp)
{
	InventoryComp = InInventoryComp;
	GridWrapBox->ClearChildren();
	CreateInventorySlots();
}

void UTPSInventoryGridWidget::CreateInventorySlots()
{
	int i = 0;
	for (const FInventorySlot& InventorySlot : InventoryComp->GetInventorySlots())
	{
		UE_LOG(LogTemp, Log, TEXT("In Loop : %d"),i);
		auto WidgetClass = InventorySlotWidget.LoadSynchronous();
		UE_LOG(LogTemp, Log, TEXT("LoadClass Done"));
		UTPSInventorySlot* NewInventorySlotWidget = CreateWidget<UTPSInventorySlot>(
			this, WidgetClass,TEXT("Inventory Slot Widget"));	
		NewInventorySlotWidget->Init(InventoryComp.Get(), InventorySlot.ItemName, InventorySlot.ItemQuantity);
		UE_LOG(LogTemp, Log, TEXT("Create and Init Done!!"));
		GridWrapBox->AddChildToWrapBox(NewInventorySlotWidget);
		UE_LOG(LogTemp, Log, TEXT("Add Done!!"));
		
		UE_LOG(LogTemp, Log, TEXT("End Loop : %d"),i);
		++i;
	}
}
