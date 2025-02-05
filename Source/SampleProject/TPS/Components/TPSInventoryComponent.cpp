#include "Components/TPSInventoryComponent.h"

#include "System/TPSCollisionChannels.h"


FItem::FItem() : Name(NAME_None), Description(FText::GetEmpty()), Thumbnail(nullptr), ItemClass(nullptr), StackSize(1)
{
}

UTPSInventoryComponent::UTPSInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTPSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTPSInventoryComponent::TraceItem()
{
	if (AActor* OwnerActor = GetOwner())
	{
		FVector TraceStart = OwnerActor->GetActorLocation();
		FVector TraceEnd = OwnerActor->GetActorLocation();
		TArray<FHitResult> Hits;
		FCollisionShape SphereShape = FCollisionShape::MakeSphere(SweepSphereRadius);
		FCollisionQueryParams CollisionQueryParameters(TEXT("Interact Item"),true, OwnerActor);
		TArray<AActor*> AttachedActors;
		OwnerActor->GetAttachedActors(OUT AttachedActors);
		CollisionQueryParameters.AddIgnoredActors(AttachedActors);

		GetWorld()->SweepMultiByChannel(Hits, TraceStart, TraceEnd, FQuat::Identity, TPS_TraceChannel_ItemInteraction,
		                                SphereShape, CollisionQueryParameters);

		if (Hits.Num() > 0)
		{
			// hit actor가 interface를 has 하고 있을경우 처리하도록 하는 것 추가 필요
		}
	}
	
	
}


// Called every frame
void UTPSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

