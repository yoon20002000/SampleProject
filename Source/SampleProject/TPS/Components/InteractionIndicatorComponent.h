#pragma once

#include "Components/ActorComponent.h"
#include "InteractionIndicatorComponent.generated.h"


class UTPSInteractionIndicatorWidget;
class ATPSInteractionActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSInteractionIndicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSInteractionIndicatorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

private:
	ATPSInteractionActor* GetInteractionActor() const;
private:
	UPROPERTY(EditAnywhere, meta=(allowPrivateAccess=true), Category="Interaction")
	float InteractionCheckRadius;
	UPROPERTY(EditAnywhere, meta=(allowPrivateAccess=true), Category="Interaction")
	float InteractionCheckRange;
	UPROPERTY(EditAnywhere, meta=(allowPrivateAccess=true), Category="Interaction")
	TSubclassOf<UUserWidget> InteractionIndicatorClass;
	UPROPERTY(Transient)
	TObjectPtr<UTPSInteractionIndicatorWidget> InteractionIndicatorWidget;
};
