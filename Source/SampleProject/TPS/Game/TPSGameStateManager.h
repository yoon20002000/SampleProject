// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TPSGameStateManager.generated.h"

class ULevelStreaming;

UENUM()
enum class EGameplayState : uint8
{
	None = 0,
	Title = 1,
	MainGame = 2,
	GameResult = 3,
	GameReplay = 4,
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
	void OpenLevel(const EGameplayState InGameState);
	
private:
	const FName TitleLevelName = TEXT("Title");
	const FName TitleUIName = TEXT("Title");
	const FName MainGameLevelName = TEXT("MainGame");
	const FName MainGameUIName = TEXT("BattleHUD");
	const FName PlayerResourceName = TEXT("Player"); 
	EGameplayState GameState;
	
	FInputModeGameOnly InputGameOnly;
	FInputModeUIOnly InputUIOnly;
};
