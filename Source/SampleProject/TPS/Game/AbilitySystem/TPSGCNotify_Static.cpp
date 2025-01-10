#include "Game/AbilitySystem/TPSGCNotify_Static.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

UTPSGCNotify_Static::UTPSGCNotify_Static()
{
}

FVector UTPSGCNotify_Static::GetSpawnLocation(const FHitResult& HitResult) const
{
	bool bBlockingHit;
	bool bInitialOverlap;
	float Time;
	float Distance;
	FVector Location;
	FVector ImpactPoint;
	FVector Normal;
	FVector ImpactNormal;
	UPhysicalMaterial* PhyMat;
	AActor* HitActor;
	UPrimitiveComponent* HitComp;
	FName HitBoneName;
	FName BoneName;
	int32 HitItem;
	int32 ElementIndex;
	FVector TraceStart;
	FVector TraceEnd;
	int32 FaceIndex;
	UGameplayStatics::BreakHitResult(HitResult, bBlockingHit, bInitialOverlap, Time, Distance, Location, ImpactPoint,
	                                 Normal, ImpactNormal, PhyMat, HitActor, HitComp, HitBoneName, BoneName, HitItem,
	                                 ElementIndex, FaceIndex, TraceStart, TraceEnd);
	return Location;
}

bool UTPSGCNotify_Static::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	float NormalizedMagnitude;
	float RawMagnitude;

	FGameplayEffectContextHandle EffectContext;
	FGameplayTag MatchedTagName;
	FGameplayTag OriginalTag;
	FGameplayTagContainer AggregatedSourceTags;
	FGameplayTagContainer AggregatedTargetTags;
	FVector Location;
	FVector Normal;
	AActor* Instigator;
	AActor* EffectCauser;
	UObject* SourceObject;
	UPhysicalMaterial* PhysicalMaterial;
	int32 GameplayEffectLevel;
	int32 AbilityLevel;
	USceneComponent* TargetAttackComponent;
	bool bReplicateLocationWhenUsingMinimalRepProxy;
	
	UAbilitySystemBlueprintLibrary::BreakGameplayCueParameters(Parameters, NormalizedMagnitude, RawMagnitude,
	                                                           EffectContext, MatchedTagName, OriginalTag,
	                                                           AggregatedSourceTags, AggregatedTargetTags, Location,
	                                                           Normal, Instigator, EffectCauser, SourceObject,
	                                                           PhysicalMaterial, GameplayEffectLevel, AbilityLevel,
	                                                           TargetAttackComponent,
	                                                           bReplicateLocationWhenUsingMinimalRepProxy);

	const FHitResult* HitResult = EffectContext.GetHitResult();

	FVector SpawnLocation = GetSpawnLocation(*HitResult);
	UGameplayStatics::SpawnEmitterAtLocation(MyTarget, Particle, SpawnLocation);

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
