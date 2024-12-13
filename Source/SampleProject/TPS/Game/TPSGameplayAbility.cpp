// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TPSGameplayAbility.h"

UTPSGameplayAbility::UTPSGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ActivationPolicy = ETPSAbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = ETPSAbilityActivationGroup::Independent;
}

ETPSAbilityActivationPolicy UTPSGameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}

ETPSAbilityActivationGroup UTPSGameplayAbility::GetActivationGroup() const
{
	return ActivationGroup;
}
