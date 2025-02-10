#include "UI/Inventory/TPSInventoryGridWidget.h"

#include "Components/TPSInventoryComponent.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "UI/TPSInventorySlot.h"

void UTPSInventoryGridWidget::Init(UTPSInventoryComponent* InInventoryComp)
{
	InventoryComp = InInventoryComp;
	InventoryComp->OnInventoryUpdatedDelegate.AddUObject(this, &ThisClass::OnUpdateInventorySlot);
	GridWrapBox->ClearChildren();
	CreateInventorySlots();
}

void UTPSInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTPSInventoryGridWidget::BeginDestroy()
{
	if (InventoryComp != nullptr)
	{
		InventoryComp->OnInventoryUpdatedDelegate.RemoveAll(this);
	}
	Super::BeginDestroy();
}

void UTPSInventoryGridWidget::CreateInventorySlots()
{
	int SlotIndex = 0;
	for (const FInventorySlot& InventorySlot : InventoryComp->GetInventorySlots())
	{
		UTPSInventorySlot* NewInventorySlotWidget = CreateWidget<UTPSInventorySlot>(
			this, InventorySlotWidget.LoadSynchronous());
		NewInventorySlotWidget->Init(InventoryComp.Get(), SlotIndex, InventorySlot.ItemName,
		                             InventorySlot.ItemQuantity);
		GridWrapBox->AddChildToWrapBox(NewInventorySlotWidget);
		++SlotIndex;
	}
}

void UTPSInventoryGridWidget::OnUpdateInventorySlot()
{
	GridWrapBox->ClearChildren();
	CreateInventorySlots();
}
