#pragma once

#include "CoreMinimal.h"
#include "TPSGameManager.generated.h"

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

private:
	void SpawnPlayer();

	UPROPERTY(Transient)
	TObjectPtr<UGameDataAsset> GameDataAsset;
	
	UPROPERTY(Transient)
	TObjectPtr<ATPSPlayer> Player;

	void GetSpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex = -1) ;
	
};
