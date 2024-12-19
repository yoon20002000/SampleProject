// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSCombatAttributeSet.h"

#include "Net/UnrealNetwork.h"

UTPSCombatAttributeSet::UTPSCombatAttributeSet() : BaseDamage(0.0f), BaseHeal(0.0f)
{
}

void UTPSCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UTPSCombatAttributeSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BaseDamage, OldValue);
}

void UTPSCombatAttributeSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BaseHeal, OldValue);
}
