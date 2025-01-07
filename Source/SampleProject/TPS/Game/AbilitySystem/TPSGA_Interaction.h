// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/TPSGameplayAbility.h"
#include "TPSGA_Interaction.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGA_Interaction : public UTPSGameplayAbility
{
	GENERATED_BODY()
public:
	UTPSGA_Interaction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(Transient)
	TObjectPtr<AActor> FocusedActor;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannels;
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float TraceRadius;
};
