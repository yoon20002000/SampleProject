// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ActorPoolingInterface.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UTPSProjectileMovementComponent;
class USphereComponent;

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
class SAMPLEPROJECT_API AProjectileBase : public AActor, public IActorPoolingInterface
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
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void LifeSpanExpired() override;
	virtual void PoolBeginPlay_Implementation() override;
	virtual void PoolEndPlay_Implementation() override;
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;
	UPROPERTY(EditdefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactVFX;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<USoundCue> ImpactSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UTPSProjectileMovementComponent> MoveComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> AudioComp;
	
};
