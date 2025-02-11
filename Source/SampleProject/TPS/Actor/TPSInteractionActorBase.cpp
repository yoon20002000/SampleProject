#include "Actor/TPSInteractionActorBase.h"

ATPSInteractionActorBase::ATPSInteractionActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ATPSInteractionActorBase::Interaction(AActor* InteractionActor)
{
	UE_LOG(LogTemp, Log, TEXT("Interaction Actor : %s"), *InteractionActor->GetName());
}

