// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "TPSHelper.h"
#include "Game/TPSGameplay.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent() : FocusedActor(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInteractionComponent::Interaction()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red, "Focused Actor is nullptr");
		return;
	}

	APawn* Player = CastChecked<APawn>(GetOwner());
	ITPSGameplay::Execute_Interact(FocusedActor, Player);
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UInteractionComponent::FindInteractableActor()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	for (auto CollisionChannel : CollisionChannels)
	{
		ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = TPSHelper::GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity,
	                                                                  ObjectQueryParams,
	                                                                  Shape);
	FColor LineColor = (bBlockingHit == true ? FColor::Green : FColor::Red);

	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.f);

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			if (HitActor->Implements<UTPSGameplay>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	// To do : UI 추가
	if (FocusedActor != nullptr)
	{
	}
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APawn* PlayerPawn = CastChecked<APawn>(GetOwner());
	if (PlayerPawn->IsLocallyControlled() == true)
	{
		FindInteractableActor();
	}
}
