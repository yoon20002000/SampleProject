

#include "TPSPawn.h"

#include "TPSSystemManager.h"

// Sets default values
ATPSPawn::ATPSPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATPSPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{	
	Super::EndPlay(EndPlayReason);	
}

// Called every frame
void ATPSPawn::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
}
