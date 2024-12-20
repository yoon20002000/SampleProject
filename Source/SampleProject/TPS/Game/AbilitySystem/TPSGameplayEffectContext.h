// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "TPSGameplayEffectContext.generated.h"

struct FGameplayEffectContextHandle;

USTRUCT()
struct FTPSGameplayEffectContext
{
	GENERATED_BODY()
public:
	FTPSGameplayEffectContext();
	FTPSGameplayEffectContext(AActor* Instigator, AActor* InEffectCauser);

	static SAMPLEPROJECT_API FTPSGameplayEffectContext* ExtractEffectContext(FGameplayEffectContextHandle Handle);

	void SetAbilitySource(const ITPSAbilitySourceInterface* InObject, float InsSourceLevel);
};
