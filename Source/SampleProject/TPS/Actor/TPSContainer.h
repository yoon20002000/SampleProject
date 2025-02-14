#pragma once

#include "Actor/TPSInteractionActorBase.h"
#include "TPSContainer.generated.h"

class UTPSContainerInventoryWidget;
class UTPSInventoryComponent;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSContainer : public ATPSInteractionActorBase
{
	GENERATED_BODY()
public:
	ATPSContainer();
	/// ITPSInteractionInterface Start
	virtual void Interaction(AActor* InteractionActor) override;
	/// ITPSInteractionInterface End
	
private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<UTPSInventoryComponent> InventoryComp;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	FName UIWidgetName;
};
