#include "Game/AbilitySystem/TPSGCNotify_Static.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

UTPSGCNotify_Static::UTPSGCNotify_Static()
{
}

FVector UTPSGCNotify_Static::GetSpawnLocation(const FHitResult& HitResult) const
{
	return HitResult.Location;
}

bool UTPSGCNotify_Static::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();

	FVector SpawnLocation = GetSpawnLocation(*HitResult);
	UGameplayStatics::SpawnEmitterAtLocation(MyTarget, Particle, SpawnLocation);

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
