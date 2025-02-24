// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/TPSCommonActivatableWidget.h"
#include "TPSTitleWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSTitleWidget : public UTPSCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickedPlayButton();
	UFUNCTION()
	void OnClickedQuitButton();

private:
	UPROPERTY(meta=(AllowPrivateAccess = true, BindWidget), BlueprintReadOnly)
	TObjectPtr<UButton> PlayButton;
	UPROPERTY(meta=(AllowPrivateAccess = true, BindWidget), BlueprintReadOnly)
	TObjectPtr<UButton> QuitButton;
};
