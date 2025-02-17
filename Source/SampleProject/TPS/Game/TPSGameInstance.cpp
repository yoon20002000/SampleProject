#include "TPSGameInstance.h"

#include "CommonUISettings.h"
#include "GameplayTagContainer.h"
#include "ICommonUIModule.h"
#include "LogTPSGame.h"
#include "TPSCommonLocalPlayer.h"
#include "TPSSystemManager.h"
#include "Kismet/GameplayStatics.h"
#include "System/SaveLoad/TPSSaveGame.h"
#include "UI/TPSUIManager.h"

void UTPSGameInstance::Init()
{
	Super::Init();
	FGameplayTagContainer PlatformTraits = ICommonUIModule::GetSettings().GetPlatformTraits();

	const int32 UserIndex = 0;
	if (UGameplayStatics::DoesSaveGameExist(SaveData_Name, UserIndex))
	{
		PlayerSaveData = Cast<UTPSSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveData_Name, UserIndex));
	}
	else
	{
		PlayerSaveData = Cast<UTPSSaveGame>(UGameplayStatics::CreateSaveGameObject(UTPSSaveGame::StaticClass()));
	}
	SaveGameToSlot(PlayerSaveData);
}

int32 UTPSGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);

	if (ReturnVal != INDEX_NONE)
	{
		if (PrimaryPlayer.IsValid() == false)
		{
			UE_LOG(LogTPSGame, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"), *NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}

		if (UTPSSystemManager* Manager = UTPSSystemManager::Get())
		{
			UTPSUIManager* UIManager = Manager->GetUIManager();
			if (UTPSCommonLocalPlayer* LocalPlayer = CastChecked<UTPSCommonLocalPlayer>(PrimaryPlayer); UIManager != nullptr )
			{
				UIManager->NotifyPlayerAdded(LocalPlayer);	
			}
		}
	}

	return ReturnVal;
}

bool UTPSGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		//TODO: do we want to fall back to another player?
		PrimaryPlayer.Reset();
		UE_LOG(LogTPSGame, Log, TEXT("RemoveLocalPlayer: Unsetting Primary Player from %s"),
		       *ExistingPlayer->GetName());
	}
	if (UTPSSystemManager* Manager = UTPSSystemManager::Get())
	{
		Manager->GetUIManager()->NotifyPlayerDestroyed(CastChecked<UTPSCommonLocalPlayer>(ExistingPlayer));
	}

	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UTPSGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
}

void UTPSGameInstance::Shutdown()
{
	UTPSSystemManager::Get()->SetWorld(nullptr);
	Super::Shutdown();
}

void UTPSGameInstance::SaveGameToSlot(UTPSSaveGame* SaveGameData)
{
	const int32 UserIndex = 0;
	UGameplayStatics::SaveGameToSlot(SaveGameData, SaveData_Name, UserIndex);
}

void UTPSGameInstance::SaveInventoryData(const TArray<FInventorySlot>& InventorySlots)
{
	GetSaveGameData()->SetInventoryData(InventorySlots);
	SaveGameToSlot(PlayerSaveData);
}

const TArray<FInventorySlot>& UTPSGameInstance::GetInventoryData()
{
	return PlayerSaveData->GetInventoryData();
}

UTPSSaveGame* UTPSGameInstance::GetSaveGameData() const
{
	return PlayerSaveData;
}
