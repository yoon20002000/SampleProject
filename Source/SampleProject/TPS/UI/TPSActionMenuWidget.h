#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSActionMenuWidget.generated.h"

class UTPSInventoryComponent;
class UButton;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSActionMenuWidget : public UTPSCommonUserWidget
{
	GENERATED_BODY()
public:
	void Init(UTPSInventoryComponent* InInventoryComp, const int32 InTargetIndex);
private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> UseButton;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> Drop1Button;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> DropAllButton;

	TWeakObjectPtr<UTPSInventoryComponent> InventoryComp;
	int32 InteractionTargetIndex;
};
