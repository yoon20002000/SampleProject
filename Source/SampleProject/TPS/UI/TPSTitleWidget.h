// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
private:
	UPROPERTY(meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> PlayButton;
	UPROPERTY(meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> QuitButton;
};
