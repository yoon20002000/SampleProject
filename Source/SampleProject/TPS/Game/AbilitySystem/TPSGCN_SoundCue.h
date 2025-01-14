// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayCueNotify_Static.h"
#include "TPSGCN_SoundCue.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGCN_SoundCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UTPSGCN_SoundCue();
	
protected:
	virtual FVector GetSpawnLocation(const FHitResult& HitResult) const;
private:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundCue> SoundCue;
};
