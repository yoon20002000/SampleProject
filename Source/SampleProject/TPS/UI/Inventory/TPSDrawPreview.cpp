#include "UI/Inventory/TPSDrawPreview.h"

#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "Components/Image.h"
#include "Components/TPSInventoryComponent.h"

void UTPSDrawPreview::SetPreview(const FName& InItemName)
{
	if (FItem* Item = UTPSSystemManager::Get()->GetGameManager()->GetItemDataOrNullptr(FName(InItemName)))
	{
		PreviewItemIconImage->SetBrushFromTexture(Item->Thumbnail);	
	}
}
