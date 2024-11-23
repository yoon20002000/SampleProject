// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();
	void Interaction();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void FindInteractableActor();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;

	UPROPERTY(Transient)
	TObjectPtr<AActor> FocusedActor;
	
};
