#pragma once

#include "UObject/Interface.h"
#include "TPSInteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTPSInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLEPROJECT_API ITPSInteractionInterface
{
	GENERATED_BODY()
public:
	virtual void Interaction(AActor* InteractionActor) = 0;
};
