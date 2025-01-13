// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSCommonUserWidget.h"
#include "Components/TPSHealthComponent.h"
#include "TPSHealthBar.generated.h"

class UTextBlock;
class UProgressBar;
class UCommonNumericTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSHealthBar : public UTPSCommonUserWidget
{
	GENERATED_BODY()
public:
	void InitHealthBar(const ATPSPlayer* TPSPlayer);
	void UninitHealthBar(const ATPSPlayer* TPSPlayer);
protected:
	virtual bool Initialize() override;
	
	virtual void NativeConstruct() override;
	virtual void BeginDestroy() override;
	UFUNCTION()
	

	void UpdateUI();
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> HealthPointText;

	float CurrentHealth;
	float CurrentMaxHealth;
};
