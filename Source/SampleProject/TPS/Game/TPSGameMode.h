// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameMode.generated.h"

enum class ETPSBalanceStatus : uint8;
class UAISpawnSubSystem;
class ATPSCharacter;
class ATPSPlayer;
enum class EGameplayState : uint8;
class UGameDataAsset;
class UTPSGameInstance;

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDifficultyUpdate, ETPSBalanceStatus);
UCLASS()
class SAMPLEPROJECT_API ATPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPSGameMode();
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void OnActorKilled(AActor* KilledActor, AActor* InstigatorActor);
	
	FORCEINLINE ETPSBalanceStatus& GetGameDifficultyStatus()
	{
		return GameDifficultyStatus;
	}
	FORCEINLINE void SetGameDifficultyStatus(ETPSBalanceStatus NewStatus)
	{
		GameDifficultyStatus = NewStatus;
		OnDifficultyUpdate.Broadcast(GameDifficultyStatus);
	}
public:
	UPROPERTY(EditAnywhere, Category=TPS)
	TSoftObjectPtr<UGameDataAsset> GameDataAsset;
// Difficulty Section
	FOnDifficultyUpdate OnDifficultyUpdate;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	EGameplayState GameplayState;
	UPROPERTY(EditAnywhere, Category=TPS, meta=(AllowPrivateAccess = true))
	TSubclassOf<ATPSPlayer> PlayerClass;
	UPROPERTY(EditAnywhere, Category=TPS, meta=(AllowPrivateAccess = true))
	TSubclassOf<ACharacter> AICharacterClass;

	TWeakObjectPtr<UAISpawnSubSystem> AISpawnSubSystem;
	
//Difficulty Section
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	ETPSBalanceStatus GameDifficultyStatus;
};
