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

void UTPSInventoryGridWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTPSInventoryGridWidget::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Log, TEXT("BeginDestroy"));
}

void UTPSInventoryGridWidget::CreateInventorySlots()
{
	for (const FInventorySlot& InventorySlot : InventoryComp->GetInventorySlots())
	{
		UTPSInventorySlot* NewInventorySlotWidget = CreateWidget<UTPSInventorySlot>(
			this, InventorySlotWidget.LoadSynchronous());	
		NewInventorySlotWidget->Init(InventoryComp.Get(), InventorySlot.ItemName, InventorySlot.ItemQuantity);
		GridWrapBox->AddChildToWrapBox(NewInventorySlotWidget);
	}
}
