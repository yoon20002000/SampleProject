#pragma once

#include "TPSHelper.h"
#include "Character/TPSPlayer.h"
#include "Components/TPSInventoryComponent.h"
#include "Data/GameDataAsset.h"
#include "TPSGameManager.generated.h"

struct FCharacterAssetInfo;
struct FWorldActorData;
struct FItem;
struct FGameTableInfo;
class ATPSCharacter;
class ATPSPlayer;
class UGameDataAsset;
class ATPSGameMode;

UENUM(BlueprintType)
enum ECharacterDataTableType
{
	PlayerCharacterData		UMETA(DisplayName = "Player Character Data"),
	AICharacterData			UMETA(DisplayName = "AI Character Data"),
};

UENUM(BlueprintType)
enum class EDataTableType : uint8
{
	ItemData		UMETA(DisplayName = "Item Data"),
	PlayerLevelData UMETA(DisplayName = "Player Level Data"),
	BalanceData		UMETA(DisplayName = "Balance Data"),
};

UCLASS()
class UTPSGameManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	void InitData(const ATPSGameMode* InGameMode);
	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	void SpawnPlayer(const FName& CharacterDataName = TEXT("Player"), const int SpawnPointIndex = -1);
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
	
	FItem* GetItemDataOrNullptr(const FName& ItemName);
	
	TArray<TObjectPtr<ATPSCharacter>>& GetAllCharacters();
	ATPSPlayer* GetPlayer();
	const UDataTable* GetGameData(const EDataTableType& InDataType);
	const FGameTableInfo& GetGameTableInfo(const EDataTableType& InDataType) const;
	const FCharacterAssetInfo& GetCharacterGameData(const ECharacterDataTableType& InCharacterDataTableTypes);

private:
	void GetSpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex = -1);
	FName GetDataTableName(const EDataTableType& InTableType) const;
	FName GetCharacterDataTableName(const ECharacterDataTableType& InCharacterDataTableType) const;
	
private:	
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
