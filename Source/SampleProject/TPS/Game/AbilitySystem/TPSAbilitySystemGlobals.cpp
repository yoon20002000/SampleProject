// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSAbilitySystemGlobals.h"

#include "TPSGameplayEffectContext.h"

FGameplayEffectContext* UTPSAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FTPSGameplayEffectContext();
}
