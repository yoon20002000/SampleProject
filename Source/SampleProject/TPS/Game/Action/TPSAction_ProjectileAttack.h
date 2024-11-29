// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Action/TPSAction.h"
#include "TPSAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSAction_ProjectileAttack : public UTPSAction
{
	GENERATED_BODY()

public:
	UTPSAction_ProjectileAttack();
	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void Attack(ACharacter* InstigatorCharacter);

protected:
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float SweepRadius;

	UPROPERTY(EditAnywhere, Category = "Targeting")
	float SweepDistanceFallback;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName GunFireSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UParticleSystem> ShootingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<USoundBase> ShootingSound;
};
