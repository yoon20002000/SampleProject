#pragma once

#include "Game/TPSInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "TPSInteractionActorBase.generated.h"

UCLASS()
class SAMPLEPROJECT_API ATPSInteractionActorBase : public AActor, public ITPSInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ATPSInteractionActorBase();

	/// ITPSInteractionInterface Start
	virtual void Interaction(AActor* InteractionActor) override;
	/// ITPSInteractionInterface End
};
