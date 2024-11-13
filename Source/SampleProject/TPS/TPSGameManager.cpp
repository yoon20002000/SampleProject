#include "TPSGameManager.h"

#include "TPSHelper.h"
#include "Game/TPSGameMode.h"
#include "Character/TPSPlayer.h"
#include "Data/GameDataAsset.h"


UTPSGameManager::UTPSGameManager(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}


void UTPSGameManager::InitData(ATPSGameMode* InGameMode)
{
	if(!InGameMode)
		return;

	if(!InGameMode->GameDataAsset.IsNull())
	{
		GameDataAsset = InGameMode->GameDataAsset.LoadSynchronous();
	}
}

void UTPSGameManager::BeginPlay()
{	
	SpawnPlayer();
}

void UTPSGameManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void UTPSGameManager::SpawnPlayer()
{
	if(!GameDataAsset)
		return;

	const FCharacterAssetInfo& assetInfo = GameDataAsset->GetCharacterData(TEXT("Player"));
	if(assetInfo.AssetName.IsEmpty())
		return;

	FActorSpawnParameters ActorSpawnParameter;
	ActorSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParameter.bDeferConstruction = true;
	ActorSpawnParameter.ObjectFlags |= RF_Transient;

	Player = TPSHelper::GetWorld()->SpawnActor<ATPSPlayer>(assetInfo.Character.LoadSynchronous(),FVector::ZeroVector, FRotator::ZeroRotator, ActorSpawnParameter);
	if(Player)
	{
		TPSHelper::GetPlayerController()->SetPawn(Player);
		TPSHelper::GetPlayerController()->Possess(Player);
	}
	
}
