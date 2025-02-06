#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSInventoryGridWidget.generated.h"

class UWrapBox;
class UTPSInventorySlot;
class UTPSInventoryComponent;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSInventoryGridWidget : public UTPSCommonUserWidget
{
	GENERATED_BODY()
public:
	void Init(UTPSInventoryComponent* InInventoryComp);
private:
	void CreateInventorySlots();
private:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UWrapBox> GridWrapBox;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TSoftClassPtr<UTPSInventorySlot> InventorySlotWidget;
	TWeakObjectPtr<UTPSInventoryComponent> InventoryComp;
};
