#pragma once

#include "Game/TPSInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "TPSItemActor.generated.h"

UCLASS()
class SAMPLEPROJECT_API ATPSItemActor : public AActor, public ITPSInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ATPSItemActor();

	/// TPSInteractionInterface Start
	virtual void LookAtInteractionActor() override;
	virtual void Interaction() override;
	/// TPSInteractionInterface End
	/// 
protected:
	virtual void BeginPlay() override;
	
};
