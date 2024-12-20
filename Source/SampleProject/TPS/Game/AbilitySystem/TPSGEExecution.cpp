// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSGEExecution.h"

#include "TPSCombatAttributeSet.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;
	FDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(UTPSCombatAttributeSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UTPSGEExecution::UTPSGEExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void UTPSGEExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	
#endif
	

	
}
