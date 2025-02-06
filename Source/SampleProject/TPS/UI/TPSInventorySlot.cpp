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
	const FItem* Item = UTPSSystemManager::Get()->GetGameManager()->GetItem(ItemName);
	
	IconImage->SetBrushFromTexture(Item->Thumbnail.LoadSynchronous());
	QuantityBox->SetVisibility(Quantity > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	QuantityText->SetCurrentValue(Quantity);
}

void UTPSInventorySlot::BeginDestroy()
{
	UE_LOG(LogTemp, Log, TEXT("Destroy Called !!! Widget Name : %s"), *this->GetName());
	Super::BeginDestroy();
}
