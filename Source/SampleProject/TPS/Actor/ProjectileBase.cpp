// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TPSProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "System/ActorPoolingSubsystem.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->SetCanEverAffectNavigation(false);
	RootComponent = SphereComp;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Effect Comp"));
	AudioComp->SetupAttachment(RootComponent);

	MoveComp = CreateDefaultSubobject<UTPSProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.0f;
	MoveComp->InitialSpeed = 8000;

	bReplicates = true;
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnActorHit);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AProjectileBase::LifeSpanExpired()
{
	UActorPoolingSubsystem::ReleaseToPool(this);
}

void AProjectileBase::PoolBeginPlay_Implementation()
{
	MoveComp->Reset();

	AudioComp->SetPaused(false);
}

void AProjectileBase::PoolEndPlay_Implementation()
{
	AudioComp->SetPaused(true);
}

void AProjectileBase::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AProjectileBase::Explode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation(),true, EPSCPoolMethod::AutoRelease);

	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

	UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), GetImpactShakeInnerRadius(),GetImpactShakeOuterRadius());

	UActorPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UActorPoolingSubsystem>();
	PoolingSubsystem->ReleaseToPool(this);
}

#if WITH_EDITOR
void AProjectileBase::MoveDataToSparseClassDataStruct() const
{
	if (UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
		BPClass == nullptr || BPClass->bIsSparseClassDataSerializable == true)
	{
		return;
	}
	
	Super::MoveDataToSparseClassDataStruct();
	
#if WITH_EDITORONLY_DATA
	FProjectileSparseData* SparseData = GetProjectileSparseData();
	SparseData->ImpactShakeInnerRadius = ImpactShakeInnerRadius_DEPRECATED;
	SparseData->ImpactShakeOuterRadius = ImpactShakeOuterRadius_DEPRECATED;
#endif
}
#endif


