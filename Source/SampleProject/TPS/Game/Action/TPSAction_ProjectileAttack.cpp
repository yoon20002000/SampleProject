// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Action/TPSAction_ProjectileAttack.h"

#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "System/TPSActorPoolingSubsystem.h"

UTPSAction_ProjectileAttack::UTPSAction_ProjectileAttack() :
	SweepRadius(20.0f),
	SweepDistanceFallback(5000),
	GunFireSocketName(TEXT("Gun_LOS"))
{
}

void UTPSAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = CastChecked<ACharacter>(Instigator);
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
}

void UTPSAction_ProjectileAttack::Attack(ACharacter* InstigatorCharacter)
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

	StopAction(InstigatorCharacter);
}
