#include "Character/TPSPlayerState.h"

#include "TPSGameManager.h"
#include "TPSPlayer.h"
#include "TPSSystemManager.h"
#include "Data/GameDataAsset.h"
#include "Data/TPSPlayerLevelData.h"
#include "Game/TPSGameMode.h"

ATPSPlayerState::ATPSPlayerState() : KillCount(0)
{
}

void ATPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	ClearKillCount();
	if (const UDataTable* DataTable = UTPSSystemManager::Get()->GetGameManager()->GetGameData(EDataTableType::PlayerLevelData))
	{
		const TArray<FName> RowNames = DataTable->GetRowNames();

		if (RowNames.Num() > 0)
		{
			FName LastRowName = RowNames.Last();
			const FString ContextString(TEXT("Get Last Row"));

			if (FTPSPlayerLevelData* LastRow = DataTable->FindRow<FTPSPlayerLevelData>(LastRowName, ContextString))
			{
				MaxLevel = LastRow->Level;
			}
		}
	}
}

void ATPSPlayerState::AddKillCount()
{
	UpdateKillCount(KillCount + 1);
}

void ATPSPlayerState::ClearKillCount()
{
	UpdateKillCount(0);
}

void ATPSPlayerState::UpdateKillCount(uint32 InKillCount)
{
	uint32 OldKillCount = KillCount;
	KillCount = InKillCount;
	uint32 Delta = KillCount - OldKillCount;

	OnKillCountChanged.Broadcast(this, KillCount, Delta);

	if (ATPSCharacter* TPSCharacter = Cast<ATPSCharacter>(GetPlayerController()->GetPawn()))
	{
		int32 CharacterLevel = TPSCharacter->GetCharacterLevel();
		int32 NextCharacterLevel = FMath::Clamp(CharacterLevel + 1, 1, MaxLevel);

		if (CharacterLevel != NextCharacterLevel)
		{
			if (const UDataTable* DataTable = UTPSSystemManager::Get()->GetGameManager()->GetGameData(EDataTableType::PlayerLevelData))
			{
				FString RowNameString = FString::Printf(TEXT("Level%d"), NextCharacterLevel);
				FName RowName = *RowNameString;
				if (FTPSPlayerLevelData* PlayerLevelData = DataTable->FindRow<FTPSPlayerLevelData>(RowName, TEXT("Player Level Data")))
				{
					if (PlayerLevelData->KillCount <= KillCount)
					{
						TPSCharacter->GetCharacterStatus()->SetLevel(NextCharacterLevel);
						
						if (ATPSGameMode* TPSGameMode = Cast<ATPSGameMode>(GetWorld()->GetAuthGameMode()))
						{
							TPSGameMode->SetGameDifficultyStatus(PlayerLevelData->BalanceType);
						}
					}
				}
			}	
		}
	}
}
