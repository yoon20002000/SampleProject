#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSInventoryGridWidget.generated.h"

class UWrapBox;
class UTPSInventorySlotWidget;
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
	virtual void NativeConstruct() override;
	virtual void BeginDestroy() override;
private:
	void CreateInventorySlots();
	void OnUpdateInventorySlot();
private:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UWrapBox> GridWrapBox;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TSoftClassPtr<UTPSInventorySlotWidget> InventorySlotWidget;
	TWeakObjectPtr<UTPSInventoryComponent> InventoryComp;
};
