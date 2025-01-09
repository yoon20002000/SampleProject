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
	
}

void UTPSGameManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (SpawnedCharacters.Num() > 0)
	{
		for (ATPSCharacter* Character : SpawnedCharacters)
		{
			if (Character!=nullptr)
			{
				Character->UninitAndDestroy();
			}
		}
	}
	SpawnedCharacters.Empty();

	if (Player!=nullptr)
	{
		Player->UninitAndDestroy();
	}
	Player= nullptr;
}

void UTPSGameManager::SpawnPlayer(const FString& CharacterDataName, const int SpawnPointIndex)
{
	if (GameDataAsset == nullptr)
	{
		return;
	}

	const FCharacterAssetInfo& assetInfo = GameDataAsset->GetCharacterData(CharacterDataName);
	if (assetInfo.AssetName.IsEmpty() == true)
	{
		return;
	}

	FActorSpawnParameters ActorSpawnParameter;
	ActorSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParameter.ObjectFlags |= RF_Transient;

	FVector SpawnPoint;
	FRotator SpawnRotation;
	GetSpawnPoint(SpawnPoint, SpawnRotation, SpawnPointIndex);
	Player = TPSHelper::GetWorld()->SpawnActor<ATPSPlayer>(assetInfo.Character.LoadSynchronous(), SpawnPoint,
	                                                       SpawnRotation, ActorSpawnParameter);
	if (Player != nullptr)
	{
		TPSHelper::GetPlayerController()->SetPawn(Player);
		TPSHelper::GetPlayerController()->Possess(Player);
	}
}

void UTPSGameManager::SpawnCharacter(const FString& CharacterDataName, const int SpawnPointIndex)
{
	if (GameDataAsset == nullptr)
	{
		return;
	}

	const FCharacterAssetInfo& AssetInfo = GameDataAsset->GetCharacterData(CharacterDataName);
	if (AssetInfo.AssetName.IsEmpty() == true)
	{
		return;
	}

	FActorSpawnParameters ActorSpawnParameter;
	ActorSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParameter.ObjectFlags |= RF_Transient;

	FVector SpawnPoint;
	FRotator SpawnRotation;
	GetSpawnPoint(SpawnPoint, SpawnRotation, SpawnPointIndex);
	ATPSCharacter* SpawnedCharacter = TPSHelper::GetWorld()->SpawnActor<ATPSCharacter>(
		AssetInfo.Character.LoadSynchronous(), SpawnPoint,
		SpawnRotation, ActorSpawnParameter);
	if (SpawnedCharacter != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawned Character : %s"), *GetNameSafe(SpawnedCharacter));
		SpawnedCharacters.Add(SpawnedCharacter);
	}
}

void UTPSGameManager::DespawnCharacter(ATPSCharacter* DespawnCharacter)
{
	SpawnedCharacters.Remove(DespawnCharacter);
}

TArray<TObjectPtr<ATPSCharacter>>& UTPSGameManager::GetAllCharacters()
{
	return SpawnedCharacters;
}

ATPSPlayer* UTPSGameManager::GetPlayer()
{
	return Player;
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

		OutPosition = PlayerStarts[InIndex]->GetActorLocation();
		OutRotator = PlayerStarts[InIndex]->GetActorRotation();
	}
}
