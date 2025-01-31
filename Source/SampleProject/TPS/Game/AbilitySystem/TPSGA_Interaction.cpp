#include "Game/AbilitySystem/TPSGA_Interaction.h"

#include "TPSHelper.h"
#include "Actor/TPSInteractionActor.h"
#include "Character/TPSPlayer.h"
#include "System/TPSCollisionChannels.h"


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
		
		ATPSPlayer* TPSPlayer = Cast<ATPSPlayer>(ActorInfo->AvatarActor);
		if (TPSPlayer == nullptr)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
		
		FCollisionQueryParams TraceParams(TEXT("Interaction Indicator Actor"), true, TPSPlayer);

		TArray<AActor*> AttachedActors;
		TPSPlayer->GetAttachedActors(AttachedActors);
		TraceParams.AddIgnoredActors(AttachedActors);
		TraceParams.AddIgnoredActor(TPSPlayer);
		TraceParams.bDebugQuery = true;

		TArray<FHitResult> HitResults;
		FVector TraceStart;
		FRotator TempRotator;
		TPSPlayer->GetActorEyesViewPoint(OUT TraceStart, OUT TempRotator);
		FVector TraceEnd = TraceStart + TPSPlayer->GetActorForwardVector() * TraceDistance;

		bool bBlockingHit = GetWorld()->LineTraceMultiByChannel(OUT HitResults, TraceStart, TraceEnd,TPS_TraceChannel_Weapon, TraceParams);
		
		FColor LineColor = (bBlockingHit == true ? FColor::Green : FColor::Red);
		
		for (FHitResult HitResult : HitResults)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceRadius, 32, LineColor, false, 0.f);

			ATPSInteractionActor* HitInteractionActor = Cast<ATPSInteractionActor>(HitResult.GetActor());
			UTPSAbilitySystemComponent* ASC = TPSPlayer->GetTPSAbilitySystemComponent();
			if (HitInteractionActor != nullptr && ASC != nullptr)
			{
				HitInteractionActor->ApplyGE(ASC);
			}
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
