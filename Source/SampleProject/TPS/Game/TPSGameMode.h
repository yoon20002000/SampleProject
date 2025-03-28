﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameMode.generated.h"

class UAISpawnSubSystem;
class ATPSCharacter;
class ATPSPlayer;
enum class EGameplayState : uint8;
class UGameDataAsset;
class UTPSGameInstance;

/**
 * 
 */
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

public:
	UPROPERTY(EditAnywhere, Category=TPS)
	TSoftObjectPtr<UGameDataAsset> GameDataAsset;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	EGameplayState GameplayState;
	UPROPERTY(EditAnywhere, Category=TPS, meta=(AllowPrivateAccess = true))
	TSubclassOf<ATPSPlayer> PlayerClass;
	UPROPERTY(EditAnywhere, Category=TPS, meta=(AllowPrivateAccess = true))
	TSubclassOf<ACharacter> AICharacterClass;

	TWeakObjectPtr<UAISpawnSubSystem> AISpawnSubSystem;
};
