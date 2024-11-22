// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TPSGameStateManager.h"

#include "TPSGameManager.h"
#include "TPSHelper.h"
#include "TPSSystemManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TPSUIManager.h"

void UTPSGameStateManager::BeginPlay()
{

	ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(TPSHelper::GetWorld(), TEXT("MainGame"));

	if (StreamingLevel != nullptr)
	{
		StreamingLevel->OnLevelLoaded.AddDynamic(this, &UTPSGameStateManager::OnLevelLoaded);
	}
	TPSGameLevels.Add(EGameplayState::MainGame, StreamingLevel);
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
				ULevelStreaming* StreamingLevel = GetStreamingLevel(EGameplayState::MainGame);
				
				StreamingLevel->SetShouldBeLoaded(true);
				StreamingLevel->SetShouldBeVisible(true);

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
				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;                    // 완료 시 호출할 객체
				LatentInfo.ExecutionFunction = FName(TEXT("OnLevelUnloaded"));    // 호출할 함수 이름
				LatentInfo.Linkage = 0;                              // 고유 값 (0 사용 가능)
				LatentInfo.UUID = __LINE__;                          // 고유 ID (보통 __LINE__ 사용)
				UTPSSystemManager::Get()->GetGameManager()->DeletePlayer();
				UTPSSystemManager::Get()->GetUIManager()->RemoveAllUIs();

				UGameplayStatics::UnloadStreamLevel(TPSHelper::GetWorld(), TEXT("MainGame"), LatentInfo, false);
				
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

	APlayerController* PC = UGameplayStatics::GetPlayerController(TPSHelper::GetWorld(), 0);
	PC->SetInputMode(InputGameOnly);
	PC->SetShowMouseCursor(false);
}

void UTPSGameStateManager::OnLevelUnloaded()
{
	SetGameplayState(EGameplayState::Title);
}

ULevelStreaming* UTPSGameStateManager::GetStreamingLevel(const EGameplayState InGameState)
{
	TObjectPtr<ULevelStreaming>* LevelStreaming = TPSGameLevels.Find(InGameState);
	if (LevelStreaming != nullptr)
	{
		return *LevelStreaming;
	}
	else
	{
		return nullptr;
	}
}
