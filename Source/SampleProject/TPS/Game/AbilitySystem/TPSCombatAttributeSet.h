// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "Game/AbilitySystem/TPSAttributeSet.h"
#include "TPSCombatAttributeSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SAMPLEPROJECT_API UTPSCombatAttributeSet : public UTPSAttributeSet
{
	GENERATED_BODY()
public:
	UTPSCombatAttributeSet();

	ATTRIBUTE_ACCESSORS(ThisClass, BaseDamage);
	ATTRIBUTE_ACCESSORS(ThisClass, BaseHeal);
	
protected:
	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);
	
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_BaseDamage, Category="TPS|Combat",
		meta=(AllowPrivateAccess= true))
	FGameplayAttributeData BaseDamage;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_BaseHeal, Category="TPS|Combat",
		meta=(AllowPrivateAccess= true))
	FGameplayAttributeData BaseHeal;
};
