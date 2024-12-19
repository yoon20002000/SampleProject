// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSAttributeSet.h"

#include "TPSAbilitySystemComponent.h"

UTPSAttributeSet::UTPSAttributeSet()
{
}

UWorld* UTPSAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);
	return Outer->GetWorld();
}

UTPSAbilitySystemComponent* UTPSAttributeSet::GetTPSAbilitySystemComponent() const
{
	return Cast<UTPSAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
