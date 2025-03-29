#include "Game/AISpawnSubSystem.h"

#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "Actor/TPSAIStart.h"
#include "Character/TPSCharacter.h"
#include "Data/GameDataAsset.h"
#include "Kismet/GameplayStatics.h"

struct FCharacterAssetInfo;
const FString UAISpawnSubSystem::MainGameTitleText = FString(TEXT("MainGame"));

UAISpawnSubSystem::UAISpawnSubSystem()
{
	SimulateCreationLimit = 5;
	CreatePeriod = 3;
}

bool UAISpawnSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	bool IsMainGameLevel = Outer->GetName().Equals(MainGameTitleText);

	return Super::ShouldCreateSubsystem(Outer) && IsMainGameLevel == true;
}

void UAISpawnSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAISpawnSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	LoadAIStartPoint();
}

void UAISpawnSubSystem::Deinitialize()
{
	Super::Deinitialize();
	StopSpawnAI();
}

void UAISpawnSubSystem::StartSpawnAI()
{
	FTimerDelegate TimerDelegate;
	FCharacterAssetInfo AICharacterAssetInfo = UTPSSystemManager::Get()->GetGameManager()->GetDataAsset()->
	                                                                     GetCharacterData(TEXT("AICharacter"));
	TimerDelegate.BindLambda(
		[this,AICharacterAssetInfo]()
		{
			if (SpawnedCharacters.Num() < SimulateCreationLimit)
			{
				SpawnAIProgress(AICharacterAssetInfo);
			}
		});

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, TimerDelegate, CreatePeriod, true);
}

void UAISpawnSubSystem::StopSpawnAI()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void UAISpawnSubSystem::DespawnCharacter(ATPSCharacter* InDespawnCharacter)
{
	SpawnedCharacters.Remove(InDespawnCharacter);
}

void UAISpawnSubSystem::SpawnAIProgress(const FCharacterAssetInfo& CharacterAssetInfo)
{
	if (CharacterAssetInfo.AssetName.IsEmpty() == true || CharacterAssetInfo.AssetName.Equals(
		FCharacterAssetInfo::Invalid.AssetName))
	{
		return;
	}

	FActorSpawnParameters ActorSpawnParameter;
	ActorSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParameter.ObjectFlags |= RF_Transient;

	FVector SpawnPoint;
	FRotator SpawnRotation;
	GetAISpawnPoint(SpawnPoint, SpawnRotation, FMath::Rand() % SpawnPoints.Num());

	ATPSCharacter* SpawnedCharacter = SpawnAICharacter(CharacterAssetInfo.Character.LoadSynchronous(), SpawnPoint,
	                                                   SpawnRotation, ActorSpawnParameter);
	if (SpawnedCharacter != nullptr)
	{
		SpawnedCharacters.Add(SpawnedCharacter);
	}
}

ATPSCharacter* UAISpawnSubSystem::SpawnAICharacter(const TSubclassOf<ATPSCharacter>& InSpawnCharacterClass,
                                                   const FVector& InSpawnLocation,
                                                   const FRotator& InSpawnRotator,
                                                   const FActorSpawnParameters& InActorSpawnParameter)
{
	ATPSCharacter* SpawnCharacter = GetWorld()->SpawnActor<ATPSCharacter>(
		InSpawnCharacterClass, InSpawnLocation, InSpawnRotator, InActorSpawnParameter);

	return SpawnCharacter;
}

void UAISpawnSubSystem::LoadAIStartPoint()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATPSAIStart::StaticClass(), SpawnPoints);

	for (auto Element : SpawnPoints)
	{
		UE_LOG(LogTemp, Log, TEXT("Element : %s"), *Element->GetName());
	}
}

void UAISpawnSubSystem::GetAISpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex)
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
