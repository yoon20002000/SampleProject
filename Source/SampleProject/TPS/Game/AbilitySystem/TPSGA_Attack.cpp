// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSGA_Attack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "System/TPSActorPoolingSubsystem.h"

UTPSGA_Attack::UTPSGA_Attack(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	SweepRadius(20.0f),
	SweepDistanceFallback(5000),
	GunFireSocketName(TEXT("Gun_LOS"))
{
}

void UTPSGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// ACharacter* Character = CastChecked<ACharacter>(Instigator);
	// Character->PlayAnimMontage(AttackAnim);

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
}

bool UTPSGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UTPSGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
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
