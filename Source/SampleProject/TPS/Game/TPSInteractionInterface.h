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
private:
	// Interaction 중 어떤 status를 줄지 어느정도 진행할지를 정하거나 다른 GA로 처리하게 하는 방법이 있을 듯 함?
	// GA or GameplayTag 관리 등등 etc 처리?
};
