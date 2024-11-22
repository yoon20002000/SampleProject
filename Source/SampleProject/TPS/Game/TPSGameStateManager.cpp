// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TPSGameStateManager.h"

#include "TPSGameManager.h"
#include "TPSHelper.h"
#include "TPSSystemManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TPSUIManager.h"

void UTPSGameStateManager::BeginPlay()
{
	SetGameplayState(EGameplayState::None);
}

void UTPSGameStateManager::SetGameplayState(EGameplayState InGameState)
{
	if (GameState != InGameState)
	{
		GameState = InGameState;

		switch (GameState)
		{
		case EGameplayState::None:
			{
				break;
			}
		case EGameplayState::Title:
			{
				UTPSSystemManager::Get()->GetUIManager()->LoadUI("Title");

				APlayerController* PC = UGameplayStatics::GetPlayerController(TPSHelper::GetWorld(), 0);
				PC->SetInputMode(InputUIOnly);
				PC->SetShowMouseCursor(true);
				break;
			}
		case EGameplayState::MainGame:
			{
				UTPSSystemManager::Get()->GetGameManager()->SpawnPlayer("Player");
				UTPSSystemManager::Get()->GetUIManager()->LoadUI("BattleHUD");

				APlayerController* PC = UGameplayStatics::GetPlayerController(TPSHelper::GetWorld(), 0);
				PC->SetInputMode(InputGameOnly);
				PC->SetShowMouseCursor(false);
				break;
			}
		case EGameplayState::GameResult:
			{
				APlayerController* PC = UGameplayStatics::GetPlayerController(TPSHelper::GetWorld(), 0);
				PC->SetInputMode(InputUIOnly);
				PC->SetShowMouseCursor(true);

				UTPSSystemManager::Get()->GetUIManager()->LoadUI("GameResult");
				break;
			}
		case EGameplayState::GameReplay:
			{
				OpenLevel(EGameplayState::Title);
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void UTPSGameStateManager::OpenLevel(const EGameplayState InGameState)
{
	FName LevelName; 
	switch (InGameState)
	{
	case EGameplayState::None:
		{
			break;
		}
	case EGameplayState::Title:
		{
			LevelName = TEXT("Title");
			break;
		}
	case EGameplayState::MainGame:
		{
			LevelName = TEXT("MainGame");
			break;
		}
	case EGameplayState::GameResult:
		{
			break;
		}
	case EGameplayState::GameReplay:
		break;
	default:
		{
			break;
		}
	}

	if (LevelName.IsNone() == false)
	{
		UGameplayStatics::OpenLevel(TPSHelper::GetWorld(), LevelName);	
	}
}
