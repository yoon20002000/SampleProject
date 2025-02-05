#pragma once

#include "Game/TPSInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "TPSItemActor.generated.h"

class UTPSItemDataComponent;

UCLASS()
class SAMPLEPROJECT_API ATPSItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATPSItemActor();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta=(allowPrivateAccess=true))
	TObjectPtr<UTPSItemDataComponent> ItemDataComponent;
};
