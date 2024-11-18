// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/TPSPlayerController.h"
#include "TPSTitlePlayerController.generated.h"
class UCommonActivatableWidget;
class UUserWidget;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSTitlePlayerController : public ATPSPlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
};
