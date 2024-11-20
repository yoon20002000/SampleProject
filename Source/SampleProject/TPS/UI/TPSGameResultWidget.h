// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UI/TPSCommonActivatableWidget.h"
#include "TPSGameResultWidget.generated.h"

class UButton;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGameResultWidget : public UTPSCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnClickedRetryButton();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> GameResultText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UCommonTextBlock> ExplainText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> RetryButton;
};
