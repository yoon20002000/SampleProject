#pragma once

#include "UI/TPSCommonActivatableWidget.h"
#include "TPSPlayerMenuWidget.generated.h"

class UTPSInventoryGridWidget;
class UTPSInventoryComponent;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSPlayerMenuWidget : public UTPSCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
private:
	TWeakObjectPtr<UTPSInventoryComponent> InventoryComp;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTPSInventoryGridWidget> InventoryGrid;
};
