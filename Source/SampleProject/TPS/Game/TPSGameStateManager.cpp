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
				
				APlayerController* PC = UGameplayStatics::GetPlayerController(TPSHelper::GetWorld(),0);
				PC->SetInputMode(InputUIOnly);
				PC->SetShowMouseCursor(true);
				break;
			}
		case EGameplayState::MainGame:
			{
				FName LevelName = TEXT("MainGame");

				ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(TPSHelper::GetWorld(), LevelName);
				if (StreamingLevel != nullptr)
				{
					StreamingLevel->OnLevelLoaded.AddDynamic(this, &UTPSGameStateManager::OnLevelLoaded);
				
					StreamingLevel->SetShouldBeLoaded(true);
					StreamingLevel->SetShouldBeVisible(true);
				}
				
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void UTPSGameStateManager::OnLevelLoaded()
{
	UTPSSystemManager::Get()->GetGameManager()->SpawnPlayer("Player");
	UTPSSystemManager::Get()->GetUIManager()->LoadUI("BattleHUD");
				
	APlayerController* PC = UGameplayStatics::GetPlayerController(TPSHelper::GetWorld(),0);
	PC->SetInputMode(InputGameOnly);
	PC->SetShowMouseCursor(false);
}
