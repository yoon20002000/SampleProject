// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Actor/ProjectileBase.h"
#include "BulletProjectile.generated.h"

class UActionEffect;

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
	TSubclassOf<UActionEffect> BleedingActionClass;
};

/**
 * 
 */
UCLASS(SparseClassDataTypes = "BulletProjectileSparseData")
class SAMPLEPROJECT_API ABulletProjectile : public AProjectileBase
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	float DamageAmount_DEPRECATED;
	TSubclassOf<UActionEffect> BleedingActionClass_DEPRECATED;
#endif
#if WITH_EDITOR
	virtual void MoveDataToSparseClassDataStruct() const override;
#endif

public:
	ABulletProjectile();
protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostInitializeComponents() override;
};
