#include "Game/AbilitySystem/TPSGA_Interaction.h"

#include "TPSHelper.h"
#include "Actor/TPSApplyGEActor.h"
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
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
		TraceParams.bDebugQuery = true;
#endif
		TArray<FHitResult> HitResults;
		FVector TraceStart;
		FRotator TempRotator;
		TPSPlayer->GetActorEyesViewPoint(OUT TraceStart, OUT TempRotator);

		APlayerCameraManager* CameraManager = TPSHelper::GetPlayerCameraManager();
		FVector CamForwardVector = CameraManager->GetActorForwardVector();
		FVector TraceEnd = TraceStart + CamForwardVector * TraceDistance;

		bool bBlockingHit = GetWorld()->LineTraceMultiByChannel(OUT HitResults, TraceStart, TraceEnd,
		                                                        TPS_TraceChannel_Interaction, TraceParams);

		DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Blue,false, 2.f);

		for (FHitResult HitResult : HitResults)
		{
			AActor* HitInteractionActor = HitResult.GetActor();
			
			if (HitInteractionActor->Implements<UTPSInteractionInterface>() == true)
			{
				if (ITPSInteractionInterface* InteractionInterface = Cast<ITPSInteractionInterface>(
					HitInteractionActor))
				{
					InteractionInterface->Interaction(TPSPlayer);
				}
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
