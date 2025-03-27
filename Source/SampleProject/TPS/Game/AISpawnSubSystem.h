#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "AISpawnSubSystem.generated.h"

class ATPSCharacter;
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
private:
	ATPSCharacter* SpawnAICharacter(const TSubclassOf<ATPSCharacter>& InSpawnCharacterClass,
	                                const FVector& InSpawnLocation,
	                                const FRotator& InSpawnRotator, const FActorSpawnParameters& InActorSpawnParameter);
};
