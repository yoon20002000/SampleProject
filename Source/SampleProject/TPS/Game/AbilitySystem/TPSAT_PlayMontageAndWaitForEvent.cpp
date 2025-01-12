// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSAT_PlayMontageAndWaitForEvent.h"

#include "AbilitySystemComponent.h"
#include "TPSAbilitySystemGlobals.h"
#include "GameFramework/Character.h"


UTPSAT_PlayMontageAndWaitForEvent* UTPSAT_PlayMontageAndWaitForEvent::CreatePlayMontageAndWaitForEvent(
	UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay,
	FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds,
	float AnimRootMotionTranslationScale)
{
	UTPSAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Rate);

	UTPSAT_PlayMontageAndWaitForEvent* Event = NewAbilityTask<UTPSAT_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	Event->MontageToPlay = MontageToPlay;
	Event->EventTags = EventTags;
	Event->Rate = Rate;
	Event->StartSection = StartSection;
	Event->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	Event->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	return Event;
}

UTPSAT_PlayMontageAndWaitForEvent::UTPSAT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer), Rate(1.f), bStopWhenAbilityEnds(true)
{
	
}

void UTPSAT_PlayMontageAndWaitForEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("UTPSAT_PlayMontageAndWaitForEvent Activate"));
	bool bPlayedMontage = false;

	if (AbilitySystemComponent.IsValid() == true)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnGameplayEvent));

			if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate,StartSection) > 0.f)
			{
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}
				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &ThisClass::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &ThisClass::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &ThisClass::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character!=nullptr &&
					(Character->GetLocalRole() == ROLE_Authority || Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}
				bPlayedMontage = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UTPSAT_PlayMontageAndWaitForEvent Anim Instance is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPSAT_PlayMontageAndWaitForEvent AbilitySystemComponent is invalid"));
	}

	if (bPlayedMontage == false)
	{
		UE_LOG(LogTemp, Log, TEXT("UTPSAT_PlayMontageAndWaitForEvent called in ability %s failed to play montage %s"),*Ability->GetName(), *GetNameSafe(MontageToPlay));
		OnCancelled.Broadcast(FGameplayTag(),FGameplayEventData());
	}
	
	SetWaitingOnAvatar();	
}

void UTPSAT_PlayMontageAndWaitForEvent::ExternalCancel()
{
	OnAbilityCancelled();
	
	Super::ExternalCancel();
}

FString UTPSAT_PlayMontageAndWaitForEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if (Ability != nullptr)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) == true? MontageToPlay.Get() : AnimInstance->GetCurrentActiveMontage();
		}
	}
	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void UTPSAT_PlayMontageAndWaitForEvent::OnDestroy(bool bInOwnerFinished)
{
	if (Ability != nullptr)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (bInOwnerFinished == true && bStopWhenAbilityEnds == true)
		{
			StopPlayingMontage();
		}
	}

	if (AbilitySystemComponent.IsValid() == true)
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

bool UTPSAT_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (ActorInfo == nullptr)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	if (AbilitySystemComponent.IsValid() == true /*&& Ability != nullptr */)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance != nullptr)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}
			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}
	
	return false;
}

void UTPSAT_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability != nullptr && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);

			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character!= nullptr && (Character->GetLocalRole() == ROLE_Authority || Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy()==EGameplayAbilityNetExecutionPolicy::LocalPredicted))
			{
				Character->SetAnimRootMotionTranslationScale(1.f);
			}
		}
	}
	if (bInterrupted == true)
	{
		if (ShouldBroadcastAbilityTaskDelegates() == true)
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates() == true)
		{
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UTPSAT_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
	if (StopPlayingMontage() == true)
	{
		if (ShouldBroadcastAbilityTaskDelegates() == true)
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UTPSAT_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted == true)
	{
		if (ShouldBroadcastAbilityTaskDelegates() == true)
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	EndTask();
}

void UTPSAT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* InData)
{
	UE_LOG(LogTemp, Log, TEXT("Event Called"));
	if (ShouldBroadcastAbilityTaskDelegates() == true)
	{
		FGameplayEventData Data = *InData;
		Data.EventTag = EventTag;
		OnEventReceived.Broadcast(EventTag, Data);
	}
}
