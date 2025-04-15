#include "Game/TPSAISpawnSubSystem.h"

#include "TPSGameManager.h"
#include "TPSGameMode.h"
#include "TPSSystemManager.h"
#include "Actor/TPSAIStart.h"
#include "Character/TPSCharacter.h"
#include "Data/GameDataAsset.h"
#include "Kismet/GameplayStatics.h"

struct FCharacterAssetInfo;
const FString UTPSAISpawnSubSystem::MainGameTitleText = FString(TEXT("MainGame"));

UTPSAISpawnSubSystem::UTPSAISpawnSubSystem()
{
	SimulateCreationLimit = 5;
	CreatePeriod = 3;
}

bool UTPSAISpawnSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	bool IsMainGameLevel = Outer->GetName().Equals(MainGameTitleText);

	return Super::ShouldCreateSubsystem(Outer) && IsMainGameLevel == true;
}

void UTPSAISpawnSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTPSAISpawnSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	LoadAIStartPoint();
}

void UTPSAISpawnSubSystem::Deinitialize()
{
	Super::Deinitialize();
	StopSpawnAI();
}

void UTPSAISpawnSubSystem::StartSpawnAI()
{
	FTimerDelegate TimerDelegate;
	
	const FCharacterAssetInfo& AICharacterAssetInfo = UTPSSystemManager::Get()->GetGameManager()->GetCharacterGameData(ECharacterDataTableType::AICharacterData);
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

void UTPSAISpawnSubSystem::StopSpawnAI()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

void UTPSAISpawnSubSystem::DespawnCharacter(ATPSCharacter* InDespawnCharacter)
{
	SpawnedCharacters.Remove(InDespawnCharacter);
}

void UTPSAISpawnSubSystem::SpawnAIProgress(const FCharacterAssetInfo& CharacterAssetInfo)
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

ATPSCharacter* UTPSAISpawnSubSystem::SpawnAICharacter(const TSubclassOf<ATPSCharacter>& InSpawnCharacterClass,
                                                   const FVector& InSpawnLocation,
                                                   const FRotator& InSpawnRotator,
                                                   const FActorSpawnParameters& InActorSpawnParameter)
{
	ATPSCharacter* SpawnCharacter = GetWorld()->SpawnActor<ATPSCharacter>(
		InSpawnCharacterClass, InSpawnLocation, InSpawnRotator, InActorSpawnParameter);

	return SpawnCharacter;
}

void UTPSAISpawnSubSystem::LoadAIStartPoint()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATPSAIStart::StaticClass(), SpawnPoints);

	for (auto Element : SpawnPoints)
	{
		UE_LOG(LogTemp, Log, TEXT("Element : %s"), *Element->GetName());
	}
}

void UTPSAISpawnSubSystem::GetAISpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex)
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

void UTPSAISpawnSubSystem::OnDifficultyChanged(ETPSBalanceStatus InBalanceStatus)
{
	UE_LOG(LogTemp, Log, TEXT("Balance Changed : %s"), *FTPSBalanceData::GetBalanceStatusName(InBalanceStatus).ToString());
	StopSpawnAI();
	
	const FGameTableInfo& TableInfo = UTPSSystemManager::Get()->GetGameManager()->GetGameTableInfo(EDataTableType::BalanceData);
	UDataTable* BalanceDataTable = TableInfo.DataTable.LoadSynchronous();
	if (BalanceDataTable != nullptr)
	{
		const FTPSBalanceData* BalanceData = BalanceDataTable->FindRow<FTPSBalanceData>(FTPSBalanceData::GetBalanceStatusName(InBalanceStatus) , TEXT("FInd Balance Data"));
		if (BalanceData != nullptr)
		{
			CreatePeriod = BalanceData->SpawnPeriod;
			SimulateCreationLimit = BalanceData->SpawnAICount;
		}
	}
	
	StartSpawnAI();
}
