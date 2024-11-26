// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

USTRUCT(BlueprintType)
struct FProjectileSparseData
{
	GENERATED_BODY()
	FProjectileSparseData() :
						ImpactShakeInnerRadius(0.f),
						ImpactShakeOuterRadius(1500.f)
	{
		
	}

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;
};

UCLASS(ABSTRACT,SparseClassDataTypes = ProjectileSparseData)
class SAMPLEPROJECT_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
	float ImpactShakeInnerRadius_DEPRECATED;
	UPROPERTY()
	float ImpactShakeOuterRadius_DEPRECATED;
	
#endif
#if WITH_EDITOR
	virtual void MoveDataToSparseClassDataStruct() const override;
#endif
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	
};
