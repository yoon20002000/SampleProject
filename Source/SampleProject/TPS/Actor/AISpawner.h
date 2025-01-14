// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AISpawner.generated.h"
class UGameDataAsset;

UCLASS()
class SAMPLEPROJECT_API AAISpawner : public AActor
{
	GENERATED_BODY()

public:
	AAISpawner();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void LoadSpawnPoint();
	void GetSpawnPoint(FVector& OutPosition, FRotator& OutRotator, int InIndex = -1);
	void SpawnCharacter(const FString& CharacterDataName = TEXT("AICharacter"), const int SpawnPointIndex = -1);
	void SpawnActor();
	void SpawningStart();

private:
	UPROPERTY(EditAnywhere, Category="Spawner", meta=(AllowPrivateAccess = true))
	FString SpawnDataAssetName;

	UPROPERTY(EditAnywhere, Category="Spawner", meta=(AllowPrivateAccess = true))
	float CreatePeriod;
	
	UPROPERTY(EditAnywhere, Category="Spawner", meta=(AllowPrivateAccess = true))
	int32 SimulateCreationLimit;


	UPROPERTY(Transient)
	TArray<AActor*> SpawnPoints;

	FTimerHandle SpawnTimerHandle;
};
