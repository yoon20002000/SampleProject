// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSGameStateManager.generated.h"

UENUM()
enum class EGameplayState : uint8
{
	None = 0,
	Title = 1,
	MainGame = 2,
};

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGameStateManager : public UObject
{
	GENERATED_BODY()
public:
	virtual void BeginPlay();
	
	void SetGameplayState(EGameplayState InGameState);
private:
	UFUNCTION()
	void OnLevelLoaded();
private:
	EGameplayState GameState;
};
