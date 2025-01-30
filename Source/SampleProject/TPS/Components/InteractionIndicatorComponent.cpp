#include "Components/InteractionIndicatorComponent.h"

#include "Actor/TPSInteractionActor.h"
#include "Blueprint/UserWidget.h"
#include "System/TPSCollisionChannels.h"
#include "UI/TPSInteractionIndicatorWidget.h"

UTPSInteractionIndicatorComponent::UTPSInteractionIndicatorComponent() : InteractionCheckRadius(200.0f), InteractionCheckRange(200.0f)
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTPSInteractionIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ATPSInteractionActor* InteractionActor = GetInteractionActor())
	{
		if (InteractionIndicatorWidget == nullptr)
		{
			InteractionIndicatorWidget = CreateWidget<UTPSInteractionIndicatorWidget>(
				GetWorld(), InteractionIndicatorClass);
		}

		if (InteractionIndicatorWidget != nullptr)
		{
			InteractionIndicatorWidget->
				SetInteractionText(FText::FromString(InteractionActor->GetInteractionDescription()));
			InteractionIndicatorWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (InteractionIndicatorWidget != nullptr)
		{
			InteractionIndicatorWidget->SetVisibility(ESlateVisibility::Hidden);	
		}
	}
}

void UTPSInteractionIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

ATPSInteractionActor* UTPSInteractionIndicatorComponent::GetInteractionActor() const
{
	AActor* OwnerActor = GetOwner();
	FCollisionQueryParams TraceParams(TEXT("Interaction Indicator Actor"), true, OwnerActor);

	TArray<AActor*> AttachedActors;
	OwnerActor->GetAttachedActors(AttachedActors);
	TraceParams.AddIgnoredActors(AttachedActors);
	TraceParams.bDebugQuery = true;

	TArray<FHitResult> HitResults;
	FVector TraceStart ;
	FRotator TempRotator;
	OwnerActor->GetActorEyesViewPoint(OUT TraceStart, OUT TempRotator);
	FVector TraceEnd = TraceStart + OwnerActor->GetActorForwardVector() * InteractionCheckRange;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(InteractionCheckRadius);

	GetWorld()->SweepMultiByChannel(OUT HitResults, TraceStart, TraceEnd, FQuat::Identity, TPS_TraceChannel_Weapon,
	                                CollisionShape, TraceParams);

	DrawDebugSphere(GetWorld(), TraceStart, InteractionCheckRadius, 32, FColor::Green, false, -1, 0, 2);
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr && HitActor->IsA(ATPSInteractionActor::StaticClass()) == true)
		{
			return Cast<ATPSInteractionActor>(HitActor);
		}
	}
	
	return nullptr;
}


