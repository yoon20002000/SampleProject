#include "Game/AbilitySystem/TPSGA_Interaction.h"

#include "TPSHelper.h"
#include "Actor/TPSInteractionActor.h"
#include "Character/TPSPlayer.h"


UTPSGA_Interaction::UTPSGA_Interaction(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UTPSGA_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == true)
	{
		FCollisionObjectQueryParams ObjectQueryParams;
		for (auto CollisionChannel : CollisionChannels)
		{
			ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
		}

		FVector EyeLocation;
		ATPSPlayer* TPSPlayer = Cast<ATPSPlayer>(ActorInfo->AvatarActor);
		if (TPSPlayer == nullptr)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}

		FRotator EyeRotation;
		TPSPlayer->GetActorEyesViewPoint(EyeLocation, EyeRotation);

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

			ATPSInteractionActor* HitInteractionActor = Cast<ATPSInteractionActor>(Hit.GetActor());
			UTPSAbilitySystemComponent* ASC = TPSPlayer->GetTPSAbilitySystemComponent();
			if (HitInteractionActor != nullptr && ASC != nullptr)
			{
				HitInteractionActor->ApplyGE(ASC);
			}
		}

		// To do : UI 추가
		if (FocusedActor != nullptr)
		{
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPSGA_Interaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                    bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
