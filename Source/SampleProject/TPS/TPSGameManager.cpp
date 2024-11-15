#include "TPSGameManager.h"

#include "TPSHelper.h"
#include "Game/TPSGameMode.h"
#include "Character/TPSPlayer.h"
#include "Data/GameDataAsset.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

UTPSGameManager::UTPSGameManager(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}


void UTPSGameManager::InitData(const ATPSGameMode* InGameMode)
{
	if (InGameMode == nullptr)
	{
		return;
	}

	if (InGameMode->GameDataAsset.IsNull() == false)
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
	if (GameDataAsset == nullptr)
	{
		return;
	}

	const FCharacterAssetInfo& assetInfo = GameDataAsset->GetCharacterData(TEXT("Player"));
	if (assetInfo.AssetName.IsEmpty() == true)
	{
		return;
	}

	FActorSpawnParameters ActorSpawnParameter;
	ActorSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParameter.ObjectFlags |= RF_Transient;

	FVector SpawnPoint;
	FRotator SpawnRotation;
	GetSpawnPoint(SpawnPoint, SpawnRotation);
	Player = TPSHelper::GetWorld()->SpawnActor<ATPSPlayer>(assetInfo.Character.LoadSynchronous(), SpawnPoint,
	                                                       SpawnRotation, ActorSpawnParameter);
	if (Player)
	{
		TPSHelper::GetPlayerController()->SetPawn(Player);
		TPSHelper::GetPlayerController()->Possess(Player);
	}
}

void UTPSGameManager::GetSpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex)
{
	OutPosition = FVector::Zero();
	OutRotator = FRotator::ZeroRotator;
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(TPSHelper::GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	if (const int Num = PlayerStarts.Num(); Num > 0)
	{
		if (InIndex < 0 || InIndex >= Num)
		{
			InIndex = FMath::RandRange(0, Num - 1);
		}
		else
		{
			InIndex = 0;
		}

		OutPosition = PlayerStarts[InIndex]->GetActorLocation();
		OutRotator = PlayerStarts[InIndex]->GetActorRotation();
	}
}
