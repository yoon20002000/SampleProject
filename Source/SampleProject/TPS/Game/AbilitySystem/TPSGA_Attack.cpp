// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSGA_Attack.h"

#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "System/TPSActorPoolingSubsystem.h"

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

UTPSGA_Attack::UTPSGA_Attack(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	SweepRadius(20.0f),
	SweepDistanceFallback(5000),
	GunFireSocketName(TEXT("Gun_LOS"))
{
	SourceBlockedTags.AddTag(TAG_WeaponFireBlocked);
}

void UTPSGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* AC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(AC);

	OnTargetDataReadyCallbackDelegateHandle = AC->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnTargetDataReadyCallback);
	
	ACharacter* Character = CastChecked<ACharacter>(CurrentActorInfo->AvatarActor);
	Character->PlayAnimMontage(AttackAnim);
	
	UGameplayStatics::SpawnEmitterAttached(
		ShootingEffect,
		Character->GetMesh(),
		GunFireSocketName,
		FVector::Zero(),
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true,
		EPSCPoolMethod::AutoRelease);
	
	UGameplayStatics::SpawnSoundAttached(ShootingSound, Character->GetMesh());
	
	if (Character->HasAuthority() == true)
	{
		Attack(Character);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	
	TimerDelegate.BindLambda([this, Handle, ActorInfo, ActivationInfo]() 
	{
		// 임시 End Montage 끝난 뒤 혹은 총 쏘는 간격 있으면 적용 필요
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	});
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,3,false);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
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

void UTPSGA_Attack::Attack(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass) == true)
	{
		FVector FireLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(GunFireSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(SweepRadius);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();

		const FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		const FVector TraceEnd = TraceStart + (TraceDirection * SweepDistanceFallback);
		FVector AdjustedTraceEnd = TraceEnd;

		TArray<FHitResult> Hits;

		if (GetWorld()->SweepMultiByChannel(Hits, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel1, Shape,
											Params))
		{
			AdjustedTraceEnd = Hits[0].ImpactPoint;
		}
#if !UE_BUILD_SHIPPING
		const float DrawDuration = 2.0f;
		//Start
		DrawDebugPoint(GetWorld(), TraceStart, 8, FColor::Green, false, DrawDuration);
		//End - possibly adjusted based on hit
		DrawDebugPoint(GetWorld(), AdjustedTraceEnd, 8, FColor::Green, false, DrawDuration);
		DrawDebugLine(GetWorld(), TraceStart, AdjustedTraceEnd, FColor::Green, false, DrawDuration);
		//End - Original
		DrawDebugPoint(GetWorld(), TraceEnd, 8, FColor::Red, false, DrawDuration);
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, DrawDuration);
#endif
		FRotator ProjRotation = (AdjustedTraceEnd - FireLocation).Rotation();

		FTransform SpawnTM = FTransform(ProjRotation, FireLocation);

		// 방향 고정 필요 어떤 방향이든 캐릭터가 보고있는 arrow를 기준으로 해야 됨.

		UTPSActorPoolingSubsystem::AcquireFromPool(this,ProjectileClass,SpawnTM,SpawnParams);
	}

	// end active 해줘야 되나?
}

void UTPSGA_Attack::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle,
	FGameplayTag GameplayTag)
{
}
