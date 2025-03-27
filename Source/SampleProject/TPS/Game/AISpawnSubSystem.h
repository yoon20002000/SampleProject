#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "AISpawnSubSystem.generated.h"

class ATPSAIStart;
class ATargetSpawnPoint;
class ATPSCharacter;
struct FCharacterAssetInfo;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UAISpawnSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

public:
	static const FString MainGameTitleText;
	
	// Spawn AI Section
private:
	void SpawnAIProgress(const FCharacterAssetInfo& CharacterAssetInfo);
	ATPSCharacter* SpawnAICharacter(const TSubclassOf<ATPSCharacter>& InSpawnCharacterClass,
									const FVector& InSpawnLocation,
									const FRotator& InSpawnRotator, const FActorSpawnParameters& InActorSpawnParameter);

	void LoadAIStartPoint();
	void GetAISpawnPoint(OUT FVector& OutPosition, OUT FRotator& OutRotator, int InIndex = -1);

private:
	UPROPERTY()
	TArray<TObjectPtr<ATPSCharacter>> SpawnedCharacters;
	UPROPERTY(Transient)
	TArray<AActor*> SpawnPoints;
};
