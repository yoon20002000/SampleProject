// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSGATargetData_SingleTargetHit.h"

#include "Game/AbilitySystem/TPSGameplayEffectContext.h"

FTPSGameplayAbilityTargetData_SingleTargetHit::FTPSGameplayAbilityTargetData_SingleTargetHit() : CartridgeID(-1)
{
}

void FTPSGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context,
	bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);
	
	if (FTPSGameplayEffectContext* TypedContext = FTPSGameplayEffectContext::ExtractEffectContext(Context))
	{
		TypedContext->CartridgeID = CartridgeID;
	}
}

bool FTPSGameplayAbilityTargetData_SingleTargetHit::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);
	Ar << CartridgeID;
	
	return true;
}

UScriptStruct* FTPSGameplayAbilityTargetData_SingleTargetHit::GetScriptStruct() const
{
	return FTPSGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
}
