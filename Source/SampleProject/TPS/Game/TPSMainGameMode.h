// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/TPSGameMode.h"
#include "TPSMainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSMainGameMode : public ATPSGameMode
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
};
