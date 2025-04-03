#include "TPSGameManager.h"

#include "TPSHelper.h"
#include "Actor/TPSContainer.h"
#include "Game/TPSGameMode.h"
#include "Character/TPSPlayer.h"
#include "Data/GameDataAsset.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TPSInventoryComponent.h"
#include "Game/TPSWorldData.h"

UTPSGameManager::UTPSGameManager(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_NormalActorData(
		TEXT("/Game/Data/DT_WorldActors.DT_WorldActors"));
	if (DT_NormalActorData.Succeeded() == true)
	{
		NormalSpawnActorData = DT_NormalActorData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ContainerActorData(
		TEXT("/Game/Data/DT_ContainerActorData.DT_ContainerActorData"));
	if (DT_ContainerActorData.Succeeded() == true)
	{
		ContainerSpawnActorData = DT_ContainerActorData.Object;
	}
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
			if (Character != nullptr)
			{
				Character->UninitAndDestroy();
			}
		}
	}
	SpawnedCharacters.Empty();

	if (Player != nullptr)
	{
		Player->UninitAndDestroy();
	}
	Player = nullptr;
}

void UTPSGameManager::SpawnPlayer(const FName& CharacterDataName, const int SpawnPointIndex)
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

void UTPSGameManager::SpawnWorldActors()
{
	SpawnNormalActors();
	SpawnContainerActors();
}

void UTPSGameManager::SpawnNormalActors()
{
	TArray<FWorldActorData*> WorldActors;

	NormalSpawnActorData->GetAllRows<FWorldActorData>(TEXT("Get World Actor"), OUT WorldActors);

	for (int Index = 0; Index < WorldActors.Num(); ++Index)
	{
		FWorldActorData* ActorData = WorldActors[Index];
		SpawnActor<AActor>(ActorData->Actor.LoadSynchronous(),
		                   ActorData->ActorTransform.GetLocation(),
		                   ActorData->ActorTransform.Rotator());
	}
}

void UTPSGameManager::SpawnContainerActors()
{
	TArray<FContainerActorData*> ContainerActorDatas;

	ContainerSpawnActorData->GetAllRows<FContainerActorData>(TEXT("Get World Actor"), OUT ContainerActorDatas);

	if (ContainerSpawnActorData == nullptr)
	{
		return;
	}

	for (int32 Index = 0; Index < ContainerActorDatas.Num(); ++Index)
	{
		FContainerActorData* ContainerActorData = ContainerActorDatas[Index];
		ATPSContainer* SpawnedActor = SpawnActor<ATPSContainer>(ContainerActorData->ContainerActor.LoadSynchronous(),
		                                                        ContainerActorData->ActorTransform.GetLocation(),
		                                                        ContainerActorData->ActorTransform.Rotator());

		if (UTPSInventoryComponent* InventoryComp = SpawnedActor->GetInventoryComponent())
		{
			InventoryComp->SetMaxSlotSize(ContainerActorData->InventoryMaxStackSize);
			for (int32 ItemIndex = 0; ItemIndex < ContainerActorData->ContainedItems.Num(); ++ItemIndex)
			{
				if (FItem* ItemData = ContainerActorData->ContainedItems[ItemIndex].Item.GetRow<FItem>(
					TEXT("Load Data")))
				{
					int32 SlotIndex = ContainerActorData->ContainedItems[ItemIndex].InventorySlotIndex;
					int32 Quantity = ContainerActorData->ContainedItems[ItemIndex].Quantity;
					InventoryComp->AddItemToSlot(SlotIndex, ItemData->Name, Quantity);
				}
			}
		}
	}
}

void UTPSGameManager::DespawnCharacter(ATPSCharacter* DespawnCharacter)
{
	SpawnedCharacters.Remove(DespawnCharacter);
}

void UTPSGameManager::AddSpawnedCharacter(ATPSCharacter* InSpawnCharacter)
{
	SpawnedCharacters.Add(InSpawnCharacter);
}

UGameDataAsset* UTPSGameManager::GetDataAsset()
{
	return GameDataAsset;
}

FItem* UTPSGameManager::GetItemDataOrNullptr(const FName& ItemName)
{
	const FGameTableInfo& ItemTableInfo = GetGameTableInfo(EDataTableType::ItemData);

	if (UDataTable* DataTable = ItemTableInfo.DataTable.LoadSynchronous())
	{
		FItem* Item = DataTable->FindRow<FItem>(ItemName,TEXT("Find Item Data"));
		return Item;
	}
	return nullptr;
}

TArray<TObjectPtr<ATPSCharacter>>& UTPSGameManager::GetAllCharacters()
{
	return SpawnedCharacters;
}

ATPSPlayer* UTPSGameManager::GetPlayer()
{
	return Player;
}

const UDataTable* UTPSGameManager::GetGameData(const EDataTableType& InDataType)
{
	if (GameDataAsset == nullptr)
	{
		return nullptr;
	}
	const FName& DataTableName = GetDataTableName(InDataType);
	if (DataTableName == NAME_None)
	{
		return nullptr;
	}

	const FGameTableInfo& GameTableInfo = GameDataAsset->GetGameTableData(DataTableName);
	return GameTableInfo.DataTable.LoadSynchronous();
}

const FGameTableInfo& UTPSGameManager::GetGameTableInfo(const EDataTableType& InDataType) const
{
	if (GameDataAsset == nullptr)
	{
		return FGameTableInfo::Invalid;
	}
	const FName& DataTableName = GetDataTableName(InDataType);

	return GameDataAsset->GetGameTableData(DataTableName);
}

const FCharacterAssetInfo& UTPSGameManager::GetCharacterGameData(const ECharacterDataTableType& InCharacterDataTableTypes)
{
	if (GameDataAsset == nullptr)
	{
		return FCharacterAssetInfo::Invalid;
	}

	const FName& CharacterDataTableName = GetCharacterDataTableName(InCharacterDataTableTypes);
	if (CharacterDataTableName == NAME_None)
	{
		return FCharacterAssetInfo::Invalid;
	}
	return GameDataAsset->GetCharacterData(CharacterDataTableName);
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

FName UTPSGameManager::GetDataTableName(const EDataTableType& InTableType) const
{
	switch (InTableType)
	{
	case EDataTableType::ItemData:
		{
			const static FName ItemGameDataTableName(TEXT("ItemData"));
			return ItemGameDataTableName;
		}
	case EDataTableType::BalanceData:
		{
			const static FName BalanceGameDataTableName(TEXT("BalanceData"));
			return BalanceGameDataTableName;
		}
	case EDataTableType::PlayerLevelData:
		{
			const static FName PlayerLevelGameDataTableName(TEXT("PlayerLevelData"));
			return PlayerLevelGameDataTableName;
		}
	default:
		{
			return NAME_None;
		}
	}
}

FName UTPSGameManager::GetCharacterDataTableName(const ECharacterDataTableType& InCharacterDataTableType) const
{
	switch (InCharacterDataTableType)
	{
	case ECharacterDataTableType::PlayerCharacterData:
		{
			const static FName CharacterDataTableName(TEXT("Player"));
			return CharacterDataTableName;
		}
	case ECharacterDataTableType::AICharacterData:
		{
			const static FName CharacterDataTableName(TEXT("AICharacter"));
			return CharacterDataTableName;
		}
	default:
		{
			return NAME_None;
		}
	}
}
