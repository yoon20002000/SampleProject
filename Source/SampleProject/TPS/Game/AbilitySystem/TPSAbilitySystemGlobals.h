// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemGlobals.h"
#include "TPSAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class SAMPLEPROJECT_API UTPSAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
