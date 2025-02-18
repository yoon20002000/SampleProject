#pragma once

#include "TPSIndicatorWidgetInterface.h"
#include "UI/TPSCommonUserWidget.h"
#include "TPSNameplate.generated.h"

struct FOnAttributeChangeData;
class UTPSHealthComponent;
class UCommonTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSNameplate : public UTPSCommonUserWidget, public ITPSIndicatorWidgetInterface
{
	GENERATED_BODY()
public:
	UTPSNameplate();

	// ITPSIndicatorWidgetInterface
	virtual void BindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor) override;
	virtual void UnbindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor) override;

	// UI
	void InitNameplate(UTPSIndicatorDescriptor* IndicatorActor);
	void UninitNameplate();
	void SetNameText(const FString& InNameText);
	
	void SetHealthBarPercent();

	// Bind On Changed
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
private:
	UPROPERTY(meta=(AllowPrivateAccess=true, BindWidget))
	TObjectPtr<UProgressBar> HPBar;
	UPROPERTY(meta=(AllowPrivateAccess=true, BindWidget))
	TObjectPtr<UCommonTextBlock> CharacterNameText;

	TWeakObjectPtr<UTPSIndicatorDescriptor> TargetIndicator;
	float CurrentHealth;
	float CurrentMaxHealth;
};
