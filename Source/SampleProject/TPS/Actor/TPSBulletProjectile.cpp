// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TPSBulletProjectile.h"


void ATPSBulletProjectile::MoveDataToSparseClassDataStruct() const
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
		//SD->BleedingActionClass = BleedingActionClass_DEPRECATED;
	}

#endif
}

ATPSBulletProjectile::ATPSBulletProjectile()
{
	InitialLifeSpan = 10.0f;
}

void ATPSBulletProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != GetInstigator())
	{
		UE_LOG(LogTemp, Log, TEXT("Overlap Other Actor : %s"), *GetNameSafe(OtherActor));
		// UTPSActionComponent* AC = OtherActor->FindComponentByClass<UTPSActionComponent>();
		// if (AC != nullptr && AC->ActiveGameplayTags.HasTag(GetParryTag()))
		// {
		// 	MoveComp->Velocity = -MoveComp->Velocity;
		// 	SetInstigator(Cast<APawn>(OtherActor));
		// 	return;
		// }
		//
		// if (TPSHelper::ApplyDirectionalDamage(GetInstigator(), OtherActor, GetDamageAmount(), SweepResult) == true)
		// {
		// 	Explode();
		// 	if (AC != nullptr && GetBleedingActionClass() && HasAuthority())
		// 	{
		// 		AC->AddAction(GetInstigator(), GetBleedingActionClass());
		// 	}
		// }
	}
}

void ATPSBulletProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnActorHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	UE_LOG(LogTemp, Log, TEXT("Hit!!!!!"));

	// if (UTPSActionComponent* AC = OtherActor->FindComponentByClass<UTPSActionComponent>();  GetBleedingActionClass() && HasAuthority())
	// {
	// 	AC->AddAction(GetInstigator(), GetBleedingActionClass());
	// }
}

void ATPSBulletProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ATPSBulletProjectile::OnActorOverlap);
}
