#include "Components/TPSInventoryComponent.h"

#include "TPSHelper.h"
#include "TPSItemDataComponent.h"
#include "Character/TPSPlayer.h"
#include "Game/TPSInteractionInterface.h"
#include "System/TPSCollisionChannels.h"


FItem::FItem() : Name(NAME_None), Description(FText::GetEmpty()), Thumbnail(nullptr), ItemClass(nullptr), StackSize(1)
{
}

UTPSInventoryComponent::UTPSInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTPSInventoryComponent::Test()
{
	if (CurHitActor != nullptr)
	{
		if (UTPSItemDataComponent* ItemDataComp = CurHitActor->GetComponentByClass<UTPSItemDataComponent>())
		{
			ItemDataComp->Interaction();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT(" CurHitActor == nullptr"));
	}
}

void UTPSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTPSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceItem();
}

void UTPSInventoryComponent::TraceItem()
{
	if (AActor* OwnerActor = GetOwner())
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		OwnerActor->GetActorEyesViewPoint(OUT EyeLocation, OUT EyeRotation);
		FVector TraceStart = EyeLocation;

		APlayerCameraManager* CameraManager = TPSHelper::GetPlayerCameraManager();
		FVector CamForwardVector = CameraManager->GetActorForwardVector();
		FVector TraceEnd = TraceStart + CamForwardVector * SweepDistance;

		TArray<FHitResult> Hits;
		FCollisionQueryParams CollisionQueryParameters(TEXT("Interact Item"), true, OwnerActor);
		TArray<AActor*> AttachedActors;
		OwnerActor->GetAttachedActors(OUT AttachedActors);
		CollisionQueryParameters.AddIgnoredActors(AttachedActors);
		GetWorld()->LineTraceMultiByChannel(Hits, TraceStart, TraceEnd,TPS_TraceChannel_ItemInteraction,
		                                    CollisionQueryParameters);

		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue);

		AActor* FirstHitActor = GetFirstHitItemInteraction(Hits);

		if (FirstHitActor == nullptr)
		{
			FCollisionShape SphereShape = FCollisionShape::MakeSphere(SweepSphereRadius);
			FVector AdditionalTraceStart = TraceStart + CameraManager->GetActorForwardVector() * SweepDistance;
			FVector AdditionalTraceEnd = AdditionalTraceStart;
			GetWorld()->SweepMultiByChannel(Hits, AdditionalTraceStart, AdditionalTraceEnd, FQuat::Identity,
			                                TPS_TraceChannel_ItemInteraction,
			                                SphereShape, CollisionQueryParameters);

			DrawDebugSphere(GetWorld(), AdditionalTraceStart, SweepSphereRadius, 12, FColor::Yellow);

			FirstHitActor = GetFirstHitItemInteraction(Hits);
		}


		if (FirstHitActor == nullptr || FirstHitActor != CurHitActor)
		{
			CurHitActor = FirstHitActor;
			if (CurHitActor != nullptr)
			{
				// ITPSInteractionInterface* InteractionInterface = Cast<ITPSInteractionInterface>(CurHitActor);
				// InteractionInterface->LookAtInteractionActor();
				// InteractionInterface->Interaction();
			}
		}
	}
}

AActor* UTPSInventoryComponent::GetFirstHitItemInteraction(const TArray<FHitResult>& Hits) const
{
	if (Hits.Num() > 0)
	{
		for (const FHitResult& Hit : Hits)
		{
			if (Hit.GetActor()->GetComponentByClass<UTPSItemDataComponent>() != nullptr)
			{
				return Hit.GetActor();
			}
		}
	}
	return nullptr;
}
