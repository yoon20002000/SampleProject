// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AISpawner.h"

#include "TPSGameManager.h"
#include "TPSHelper.h"
#include "TPSSystemManager.h"
#include "Character/TPSCharacter.h"
#include "Data/GameDataAsset.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAISpawner::AAISpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();
	LoadSpawnPoint();
	
	SpawningStart();
}

void AAISpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void AAISpawner::LoadSpawnPoint()
{
	UGameplayStatics::GetAllActorsOfClass(TPSHelper::GetWorld(), APlayerStart::StaticClass(), SpawnPoints);
}

void AAISpawner::GetSpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex)
{
	OutPosition = FVector::Zero();
	OutRotator = FRotator::ZeroRotator;

	if (const int Num = SpawnPoints.Num(); Num > 0)
	{
		if (InIndex < 0 || InIndex >= Num)
		{
			InIndex = FMath::RandRange(0, Num - 1);
		}

		OutPosition = SpawnPoints[InIndex]->GetActorLocation();
		OutRotator = SpawnPoints[InIndex]->GetActorRotation();
	}
}

void AAISpawner::SpawnCharacter(const FString& CharacterDataName, const int SpawnPointIndex)
{
	if (UGameDataAsset* GameDataAsset = UTPSSystemManager::Get()->GetGameManager()->GetDataAsset())
	{
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
			UTPSSystemManager::Get()->GetGameManager()->AddSpawnedCharacter(SpawnedCharacter);
		}
	}
}

void AAISpawner::SpawnActor()
{
	if (UTPSGameManager* GM = UTPSSystemManager::Get()->GetGameManager())
	{
		if (GM->GetAllCharacters().Num() < SimulateCreationLimit)
		{
			SpawnCharacter(SpawnDataAssetName);
		}
	}
}

void AAISpawner::SpawningStart()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::SpawnActor, CreatePeriod, true);
}
