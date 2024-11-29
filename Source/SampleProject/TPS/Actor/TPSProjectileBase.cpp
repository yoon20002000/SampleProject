// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TPSProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TPSProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "System/TPSActorPoolingSubsystem.h"

// Sets default values
ATPSProjectileBase::ATPSProjectileBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	RootComponent = MeshComp;
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Effect Comp"));
	AudioComp->SetupAttachment(RootComponent);

	MoveComp = CreateDefaultSubobject<UTPSProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 0.0f;
	MoveComp->InitialSpeed = 8000;

	bReplicates = true;
}

void ATPSProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentHit.AddDynamic(this, &ATPSProjectileBase::OnActorHit);
}

// Called when the game starts or when spawned
void ATPSProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATPSProjectileBase::LifeSpanExpired()
{
	UTPSActorPoolingSubsystem::ReleaseToPool(this);
}

void ATPSProjectileBase::PoolBeginPlay_Implementation()
{
	MoveComp->Reset();

	AudioComp->SetPaused(false);
}

void ATPSProjectileBase::PoolEndPlay_Implementation()
{
	AudioComp->SetPaused(true);
}

void ATPSProjectileBase::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ATPSProjectileBase::Explode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation(),true, EPSCPoolMethod::AutoRelease);

	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

	UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), GetImpactShakeInnerRadius(),GetImpactShakeOuterRadius());

	UTPSActorPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UTPSActorPoolingSubsystem>();
	PoolingSubsystem->ReleaseToPool(this);
}

#if WITH_EDITOR
void ATPSProjectileBase::MoveDataToSparseClassDataStruct() const
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


