// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TPSPlayerController.h"
#include "TPSGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSGamePlayerController : public ATPSPlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
};
