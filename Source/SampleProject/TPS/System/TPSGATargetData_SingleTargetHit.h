// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "TPSGATargetData_SingleTargetHit.generated.h"

USTRUCT()
struct FTPSGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
	FTPSGameplayAbilityTargetData_SingleTargetHit();

	virtual void AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const override;
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
	virtual UScriptStruct* GetScriptStruct() const override;
	
	UPROPERTY()
	int32 CartridgeID;
};

template<>
struct TStructOpsTypeTraits<FTPSGameplayAbilityTargetData_SingleTargetHit> : public TStructOpsTypeTraitsBase2<FTPSGameplayAbilityTargetData_SingleTargetHit>
{
	enum
	{
		WithNetSerializer = true	
	};
};
