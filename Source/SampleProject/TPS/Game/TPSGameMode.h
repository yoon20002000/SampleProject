// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameMode.generated.h"

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
	virtual void StartPlay() override;
	virtual void BeginDestroy() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, Category=TPS)
	TSoftObjectPtr<UGameDataAsset> GameDataAsset;
};
