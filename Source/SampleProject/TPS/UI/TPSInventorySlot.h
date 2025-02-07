#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSInventorySlot.generated.h"

class UTPSDragDropOperation;
class UButton;
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
	virtual bool Initialize() override;
	void Init(UTPSInventoryComponent* InInventoryComp, const int32 InSlotIndex, const FName& InItemName, const int32 ItemQuantity);
	void UpdateInventorySlot(const FName& InItemName, const int32 Quantity);
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
private:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> SlotButton;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UBorder> SlotBoarder;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UImage> IconImage;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<USizeBox> QuantityBox;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonNumericTextBlock> QuantityText;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TSoftClassPtr<UUserWidget> DragPreviewClass;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TSoftClassPtr<UTPSDragDropOperation> DragDropOperationClass;
	
	int32 InventorySlotIndex;
	FName ItemName;
	TWeakObjectPtr<UTPSInventoryComponent> InventoryComp;
};
