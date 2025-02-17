#pragma once

#include "TPSHelper.h"
#include "Character/TPSPlayer.h"
#include "Components/TPSInventoryComponent.h"
#include "TPSGameManager.generated.h"

struct FWorldActorData;
struct FItem;
struct FGameTableInfo;
class ATPSCharacter;
class ATPSPlayer;
class UGameDataAsset;
class ATPSGameMode;

UCLASS()
class UTPSGameManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitData(const ATPSGameMode* InGameMode);
	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	void SpawnPlayer(const FString& CharacterDataName = TEXT("Player"), const int SpawnPointIndex = -1);
	void SpawnWorldActors();
	void SpawnNormalActors();
	void SpawnContainerActors();
	template<typename T>
	T* SpawnActor(UClass* SpawnClass, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator)
	{
		FActorSpawnParameters ActorSpawnParameter;
		ActorSpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParameter.ObjectFlags |= RF_Transient;
	
		return TPSHelper::GetWorld()->SpawnActor<T>(SpawnClass, SpawnLocation, SpawnRotation, ActorSpawnParameter);
	}
	template<typename T>
	T* SpawnItemActor(const FName& ItemName, const FVector& SpawnLocation = FVector::ZeroVector, const FRotator& SpawnRotation = FRotator::ZeroRotator)
	{
		FItem* ItemData = GetItemDataOrNullptr(ItemName);
		if (ItemData == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Item Data is null"));
			return nullptr;
		}
	
		return SpawnActor<T>(ItemData->ItemClass.LoadSynchronous(), SpawnLocation, SpawnRotation);
	}
	void DespawnCharacter(ATPSCharacter* DespawnCharacter);
	void AddSpawnedCharacter(ATPSCharacter* InSpawnCharacter);
	UGameDataAsset* GetDataAsset();

	const FGameTableInfo* GetItemTable() const;
	FItem* GetItemDataOrNullptr(const FName& ItemName);
	
	TArray<TObjectPtr<ATPSCharacter>>& GetAllCharacters();
	ATPSPlayer* GetPlayer();
private:
	void GetSpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex = -1);

private:
	const FString ItemDataTableName = TEXT("ItemData");

	UPROPERTY(Transient)
	TObjectPtr<UGameDataAsset> GameDataAsset;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> NormalSpawnActorData;

	UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UDataTable> ContainerSpawnActorData;
	
	UPROPERTY(Transient)
	TObjectPtr<ATPSPlayer> Player;

	UPROPERTY()
	TArray<TObjectPtr<ATPSCharacter>> SpawnedCharacters;
};
