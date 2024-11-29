// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Actor/TPSProjectileBase.h"
#include "TPSBulletProjectile.generated.h"

class UTPSActionEffect;

USTRUCT(BlueprintType)
struct FBulletProjectileSparseData : public FProjectileSparseData
{
	GENERATED_BODY()
public:
	FBulletProjectileSparseData() : DamageAmount(20.f)
	{
		
	}
public:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UTPSActionEffect> BleedingActionClass;
};

/**
 * 
 */
UCLASS(SparseClassDataTypes = "BulletProjectileSparseData")
class SAMPLEPROJECT_API ATPSBulletProjectile : public ATPSProjectileBase
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	float DamageAmount_DEPRECATED;
	TSubclassOf<UTPSActionEffect> BleedingActionClass_DEPRECATED;
#endif
#if WITH_EDITOR
	virtual void MoveDataToSparseClassDataStruct() const override;
#endif

public:
	ATPSBulletProjectile();
protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void PostInitializeComponents() override;
};
