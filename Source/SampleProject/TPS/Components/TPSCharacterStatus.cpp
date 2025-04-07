#include "Components/TPSCharacterStatus.h"

UTPSCharacterStatus::UTPSCharacterStatus()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Level = 1;
}


void UTPSCharacterStatus::BeginPlay()
{
	Super::BeginPlay();

	// ...

	
}
