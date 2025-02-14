#include "UI/TPSDD_InventorySlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TPSInventoryComponent.h"
#include "Components/Widget.h"

void UTPSDD_InventorySlot::Init(UTPSInventoryComponent* InInventoryComponent, const int32 InContentIndex)                               
{
	InventoryComp = InInventoryComponent;
	ContentIndex = InContentIndex;
}

void UTPSDD_InventorySlot::AddOnCancelledCallback(TFunction<void()> InCallback)
{
	if (InCallback != nullptr)
	{
		OnCancelled.AddLambda(InCallback);	
	}
}

int32 UTPSDD_InventorySlot::GetContentIndex() const
{
	return ContentIndex;
}

UTPSInventoryComponent* UTPSDD_InventorySlot::GetInventoryComponent() const
{
	return InventoryComp.Get();
}

void UTPSDD_InventorySlot::DragCancelled_Implementation(const FPointerEvent& PointerEvent)
{
	Super::DragCancelled_Implementation(PointerEvent);

	OnCancelled.Broadcast();
}
void UTPSDD_InventorySlot::BeginDestroy()
{
	OnCancelled.Clear();
	Super::BeginDestroy();
}
