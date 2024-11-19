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
				PC->SetInputMode(FInputModeUIOnly());
				PC->SetShowMouseCursor(true);
				break;
			}
		case EGameplayState::MainGame:
			{
				UTPSSystemManager::Get()->GetGameManager()->SpawnPlayer("Player");
				UTPSSystemManager::Get()->GetUIManager()->LoadUI("BattleHUD");
				
				APlayerController* PC = UGameplayStatics::GetPlayerController(TPSHelper::GetWorld(),0);
				PC->SetInputMode(FInputModeGameOnly());
				PC->SetShowMouseCursor(false);
				break;
			}
		default:
			{
				break;
			}
		}
	}
}
