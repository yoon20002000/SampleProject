// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/TPSGameplay.h"
#include "GameFramework/Actor.h"
#include "InteractionActor.generated.h"

class USphereComponent;

UCLASS()
class SAMPLEPROJECT_API AInteractionActor : public AActor, public ITPSGameplay
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractionActor();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
