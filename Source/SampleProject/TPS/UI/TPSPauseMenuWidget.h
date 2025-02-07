// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TPSCommonActivatableWidget.h"
#include "TPSPauseMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSPauseMenuWidget : public UTPSCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;
	UFUNCTION()
	void OnClickedOption();
	UFUNCTION()
	void OnClickedQuit();
	UFUNCTION()
	void OnClickedContinue();
	void SetGamePause(const bool bPauseGame);
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> OptionButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> QuitButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> ContinueButton;
	
};
