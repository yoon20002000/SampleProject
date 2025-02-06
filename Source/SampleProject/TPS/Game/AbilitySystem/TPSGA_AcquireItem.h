// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/TPSGameplayAbility.h"
#include "TPSGA_AcquireItem.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGA_AcquireItem : public UTPSGameplayAbility
{
	GENERATED_BODY()
public:
	UTPSGA_AcquireItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
