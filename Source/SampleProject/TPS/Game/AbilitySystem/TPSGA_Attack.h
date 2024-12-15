// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/TPSGameplayAbility.h"
#include "TPSGA_Attack.generated.h"
UENUM(BlueprintType)
enum class ETPSAbilityTargetingSource : uint8
{
	CameraTowardsFocus,
	PawnForward,
	PawnTowardsFocus,
	WeaponForward,
	WeaponTowardsFocus,
	Custom
};
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGA_Attack : public UTPSGameplayAbility
{
	GENERATED_BODY()
public:
	UTPSGA_Attack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UFUNCTION()
	void Attack(ACharacter* InstigatorCharacter);
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag);
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

private:
	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;
};
