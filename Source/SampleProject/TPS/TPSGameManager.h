#pragma once

#include "CoreMinimal.h"
#include "TPSGameManager.generated.h"

struct FItem;
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
	void DespawnCharacter(ATPSCharacter* DespawnCharacter);
	void AddSpawnedCharacter(ATPSCharacter* InSpawnCharacter);
	UGameDataAsset* GetDataAsset();
	
	FItem* GetItem(const FName& ItemName);
	
	TArray<TObjectPtr<ATPSCharacter>>& GetAllCharacters();
	ATPSPlayer* GetPlayer();
private:
	void GetSpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex = -1);

private:
	const FString ItemDataTableName = TEXT("ItemData");
	UPROPERTY(Transient)
	TObjectPtr<UGameDataAsset> GameDataAsset;

	UPROPERTY(Transient)
	TObjectPtr<ATPSPlayer> Player;

	UPROPERTY()
	TArray<TObjectPtr<ATPSCharacter>> SpawnedCharacters;
};
