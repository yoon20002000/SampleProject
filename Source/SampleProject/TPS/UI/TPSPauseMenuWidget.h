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
	UFUNCTION()
	void OnClickedOption();
	UFUNCTION()
	void OnClickedQuit();
	UFUNCTION()
	void OnClickedContinue();
	void SetMouseCursor(const bool bShowMouseCursor);
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> OptionButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> QuitButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UButton> ContinueButton;
	
};
