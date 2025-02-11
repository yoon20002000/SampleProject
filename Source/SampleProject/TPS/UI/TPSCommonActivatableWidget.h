// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "TPSCommonActivatableWidget.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSCommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
protected:
	void SetInputUIMode(bool bShowCursor = false);
	void SetInputGameMode(bool bShowCursor = false);
};
