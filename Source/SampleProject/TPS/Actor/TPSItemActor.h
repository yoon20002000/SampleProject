#pragma once

#include "Game/TPSInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "TPSItemActor.generated.h"

class UTPSItemDataComponent;

UCLASS()
class SAMPLEPROJECT_API ATPSItemActor : public AActor, public ITPSInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ATPSItemActor();
	/// TPSInteractionInterface Start
	virtual void Interaction(AActor* InteractionActor) override;
	/// TPSInteractionInterface End
private:
	UPROPERTY(EditDefaultsOnly, meta=(allowPrivateAccess=true))
	TObjectPtr<UTPSItemDataComponent> ItemDataComponent;
};
