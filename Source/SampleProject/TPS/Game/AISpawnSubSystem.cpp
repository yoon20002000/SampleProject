#include "Game/AISpawnSubSystem.h"

#include "Character/TPSCharacter.h"

const FString UAISpawnSubSystem::MainGameTitleText = FString(TEXT("MainGame"));

bool UAISpawnSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	bool IsMainGameLevel = Outer->GetName().Equals(MainGameTitleText);
	
	return Super::ShouldCreateSubsystem(Outer) && IsMainGameLevel == true;
}

void UAISpawnSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAISpawnSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

void UAISpawnSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

ATPSCharacter* UAISpawnSubSystem::SpawnAICharacter(const TSubclassOf<ATPSCharacter>& InSpawnCharacterClass, const FVector& InSpawnLocation,
                                                   const FRotator& InSpawnRotator, const FActorSpawnParameters& InActorSpawnParameter)
{
	ATPSCharacter* SpawnCharacter = GetWorld()->SpawnActor<ATPSCharacter>(InSpawnCharacterClass, InSpawnLocation, InSpawnRotator, InActorSpawnParameter);
	
	return SpawnCharacter;
}
