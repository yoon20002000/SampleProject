#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSInventorySlot.generated.h"

struct FItem;
class USizeBox;
class UTPSInventoryComponent;
class UCommonNumericTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSInventorySlot : public UTPSCommonUserWidget
{
	GENERATED_BODY()
public:
	void Init(UTPSInventoryComponent* InInventoryComp, const FName& ItemName, const int32 ItemQuantity);
	void UpdateInventorySlot(const FName& ItemName, const int32 Quantity);
private:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UBorder> SlotBoarder;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UImage> IconImage;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<USizeBox> QuantityBox;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonNumericTextBlock> QuantityText;

	TWeakObjectPtr<UTPSInventoryComponent> InventoryComp;
};
