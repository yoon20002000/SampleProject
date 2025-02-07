#include "UI/TPSInventorySlot.h"

#include "CommonNumericTextBlock.h"
#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TPSInventoryComponent.h"

void UTPSInventorySlot::Init(UTPSInventoryComponent* InInventoryComp, const FName& ItemName, const int32 ItemQuantity)
{
	InventoryComp = InInventoryComp;
	UpdateInventorySlot(ItemName,ItemQuantity);
}

void UTPSInventorySlot::UpdateInventorySlot(const FName& ItemName, const int32 Quantity)
{
	if (ItemName == NAME_None)
	{
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
		QuantityBox->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	if (const FItem* Item = UTPSSystemManager::Get()->GetGameManager()->GetItem(ItemName))
	{
		if (Item->Thumbnail != nullptr)
		{
			IconImage->SetBrushFromTexture(Item->Thumbnail);
			IconImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	QuantityBox->SetVisibility(Quantity > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	QuantityText->SetCurrentValue(Quantity);
}
