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
				UTPSSystemManager::Get()->GetUIManager()->LoadUI(TitleUIName, EUILayerType::Menu);

				APlayerController* PC = UGameplayStatics::GetPlayerController(TPSHelper::GetWorld(), 0);
				PC->SetInputMode(InputUIOnly);
				PC->SetShowMouseCursor(true);
				break;
			}
		case EGameplayState::MainGame:
			{
				UTPSSystemManager::Get()->GetUIManager()->LoadUI(MainGameUIName, EUILayerType::GameLayer);
				UTPSSystemManager::Get()->GetGameManager()->SpawnWorldActors();
				UTPSSystemManager::Get()->GetGameManager()->SpawnPlayer(PlayerResourceName, 0);

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
			LevelName = TitleLevelName;
			break;
		}
	case EGameplayState::MainGame:
		{
			LevelName = MainGameLevelName;
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
