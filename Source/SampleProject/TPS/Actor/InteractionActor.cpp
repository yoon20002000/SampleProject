// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InteractionActor.h"

#include "Components/SphereComponent.h"

// Sets default values
AInteractionActor::AInteractionActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetCollisionProfileName("InteractionObject");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetupAttachment(RootComponent);

	
}

void AInteractionActor::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

// Called when the game starts or when spawned
void AInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}
