// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BulletProjectile.h"

#include "TPSHelper.h"
#include "Components/SphereComponent.h"
#include "Components/TPSProjectileMovementComponent.h"
#include "Game/Action/ActionComponent.h"
#include "Game/Action/ActionEffect.h"

void ABulletProjectile::MoveDataToSparseClassDataStruct() const
{
	const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass == nullptr || BPClass->bIsSparseClassDataSerializable == true)
	{
		return;
	}

	Super::MoveDataToSparseClassDataStruct();

#if WITH_EDITORONLY_DATA
	if (FBulletProjectileSparseData* SD = GetBulletProjectileSparseData())
	{
		SD->DamageAmount = DamageAmount_DEPRECATED;
		SD->BleedingActionClass = BleedingActionClass_DEPRECATED;
	}

#endif
}

ABulletProjectile::ABulletProjectile()
{
	InitialLifeSpan = 10.0f;
}

void ABulletProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != GetInstigator())
	{
		UActionComponent* AC = OtherActor->FindComponentByClass<UActionComponent>();
		if (AC != nullptr && AC->ActiveGameplayTags.HasTag(GetParryTag()))
		{
			MoveComp->Velocity = -MoveComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (TPSHelper::ApplyDirectionalDamage(GetInstigator(), OtherActor, GetDamageAmount(), SweepResult) == true)
		{
			Explode();
			if (AC != nullptr && GetBleedingActionClass() && HasAuthority())
			{
				AC->AddAction(GetInstigator(), GetBleedingActionClass());
			}
		}
	}
}

void ABulletProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletProjectile::OnActorOverlap);
}
