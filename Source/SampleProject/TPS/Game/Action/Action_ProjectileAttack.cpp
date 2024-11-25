// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Action/Action_ProjectileAttack.h"

#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UAction_ProjectileAttack::UAction_ProjectileAttack() :
	SweepRadius(20.0f),
	SweepDistanceFallback(5000),
	GunFireSocketName(TEXT("Gun_LOS")),
	AttackAnimDelay(.2f)
{
}

void UAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
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
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

void UAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
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
		const float DrawDuration = 5.0f;
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

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
