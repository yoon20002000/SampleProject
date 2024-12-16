// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSGA_Attack.h"

#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "System/TPSActorPoolingSubsystem.h"
#include "System/TPSCollisionChannels.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_WeaponFireBlocked, "Ability.Weapon.NoFiring");

FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;

		FRotator Rot = Dir.Rotation();
		FQuat DirQuat(Rot);
		FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuat(FRotator(0.0f,0.0f,AngleAround));
		FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;

		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}

UTPSGA_Attack::UTPSGA_Attack(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SourceBlockedTags.AddTag(TAG_WeaponFireBlocked);
}

void UTPSGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* AC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(AC);

	OnTargetDataReadyCallbackDelegateHandle = AC->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnTargetDataReadyCallback);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

bool UTPSGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags); 
	// 이후 무기를 추가시 추가 조건 할 것
	return bResult; 
}

void UTPSGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo) == true)
	{
		if (ScopeLockCount > 0)
		{
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &ThisClass::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));;
			return;
		}

		UAbilitySystemComponent* AC = CurrentActorInfo->AbilitySystemComponent.Get();
		check(AC);

		FAbilityTargetDataSetDelegate& ATDSD = AC->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
		ATDSD.Remove(OnTargetDataReadyCallbackDelegateHandle);
		AC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

int32 UTPSGA_Attack::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int Index = 0; Index < HitResults.Num(); ++Index)
	{
		const FHitResult& HitResult = HitResults[Index];
		if (HitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()) == true)
		{
			return Index;
		}
		else
		{
			AActor* HitActor = HitResult.HitObjectHandle.FetchActor();
			if (HitActor != nullptr && HitActor->GetAttachParentActor() != nullptr && Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr)
			{
				return Index;
			}
		}
	}
	return INDEX_NONE;
}

FHitResult UTPSGA_Attack::WeaponTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius,
	bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;
	AddAdditionalTraceIgnoreActors(TraceParams);
	TraceParams.bDebugQuery = true;

	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);

	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, TraceStart, TraceEnd,FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, TraceChannel, TraceParams);
	}

	FHitResult Hit(ForceInit);
	if (HitResults.Num() > 0)
	{
		for (FHitResult& HitResult : HitResults)
		{
			auto Pred = [&HitResult](const FHitResult& Other)
			{
				return Other.HitObjectHandle == HitResult.HitObjectHandle;	
			};

			if (OutHitResults.ContainsByPredicate(Pred) == false)
			{
				OutHitResults.Add(HitResult);
			}
		}
		Hit = OutHitResults.Last();
	}
	else
	{
		Hit.TraceStart = TraceStart;
		Hit.TraceEnd = TraceEnd;
	}
	
	return Hit;
}

FHitResult UTPSGA_Attack::SingleBulletTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius,
	bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	// Draw Debug
	{
		float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(),TraceStart,TraceEnd,FColor::Red, false, 1,0,DebugThickness);
	}

	FHitResult Impact;

	// Not use SweepRadius
	if (FindFirstPawnHitResult(OutHitResults) == INDEX_NONE)
	{
		Impact = WeaponTrace(TraceStart, TraceEnd, 0.0f, bIsSimulated, OutHitResults);
	}

	if (FindFirstPawnHitResult(OutHitResults) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(TraceStart, TraceEnd, SweepRadius, bIsSimulated, SweepHits);

			const int32 FirstPawnIndex = FindFirstPawnHitResult(OutHitResults);
			if (SweepHits.IsValidIndex(FirstPawnIndex) == true)
			{
				bool bUseSweepHits = true;
				for (int32 Index = 0; Index < FirstPawnIndex; ++Index)
				{
					const FHitResult& HitResult = SweepHits[Index];

					auto Pred = [&HitResult](const FHitResult& Other)
					{
						return Other.HitObjectHandle == HitResult.HitObjectHandle;
					};
					if (HitResult.bBlockingHit == true && OutHitResults.ContainsByPredicate(Pred) == true)
					{
						bUseSweepHits = false;
						break;
					}
				}
				if (bUseSweepHits == true)
				{
					OutHitResults = SweepHits;
				}
			}
		}
	}
	return Impact;
}

void UTPSGA_Attack::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHitResults)
{
}

void UTPSGA_Attack::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;
		Avatar->GetAttachedActors(AttachedActors);
		TraceParams.AddIgnoredActors(AttachedActors);
	}
}
ECollisionChannel UTPSGA_Attack::DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const
{
	return TPS_TraceChannel_Weapon;
}

void UTPSGA_Attack::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle,
	FGameplayTag GameplayTag)
{
}
