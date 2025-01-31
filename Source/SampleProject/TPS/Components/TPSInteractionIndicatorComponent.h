#pragma once

#include "Components/ActorComponent.h"
#include "TPSInteractionIndicatorComponent.generated.h"


class UTPSIndicatorDescriptor;
enum class EActorCanvasProjectionMode : uint8;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void RegistInteractionIndicator(AActor* InteractionActor, TSoftClassPtr<UUserWidget> InIndicatorClass,EActorCanvasProjectionMode InProjectionMode, FName InSocketName, FVector InBoundingBoxAnchor, EHorizontalAlignment InHAlign, EVerticalAlignment InVAlign, bool bCanAutomaticallyRemove, bool bIsClampToScreen);
	void UnregistInteractionIndicator();
private:
	ATPSInteractionActor* GetInteractionActor() const;
private:
	UPROPERTY(EditAnywhere, meta=(allowPrivateAccess=true), Category="Interaction")
	float InteractionCheckRadius;
	UPROPERTY(EditAnywhere, meta=(allowPrivateAccess=true), Category="Interaction")
	float InteractionCheckRange;
	UPROPERTY(EditAnywhere, meta=(allowPrivateAccess=true), Category="Interaction")
	TSoftClassPtr<UUserWidget> InteractionIndicatorClass;

	UPROPERTY()
	TWeakObjectPtr<UTPSIndicatorDescriptor> CurIndicator;
};
