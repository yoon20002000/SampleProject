#pragma once

#include "GameplayCueNotify_Static.h"
#include "TPSGCNotify_Static.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGCNotify_Static : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UTPSGCNotify_Static();
protected:
	FVector GetSpawnLocation(const FHitResult& HitResult) const;
private:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> Particle;
};
