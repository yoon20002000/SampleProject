// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSCommonUserWidget.h"
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
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void BeginDestroy() override;
	UFUNCTION()
	virtual void UpdateHealthBar(float NewHealth, float MaxHealth);
	virtual void UpdateHealthPoint(float NewHealth);
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> HealthPointText;
};
