// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSGameplayEffectContext.h"

#include "GameplayEffectTypes.h"
#include "TPSAbilitySourceInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TPSGameplayEffectContext)

FTPSGameplayEffectContext::FTPSGameplayEffectContext() : FGameplayEffectContext()
{
}

FTPSGameplayEffectContext::FTPSGameplayEffectContext(AActor* Instigator, AActor* InEffectCauser) : FGameplayEffectContext(Instigator, InEffectCauser)
{
}

FTPSGameplayEffectContext* FTPSGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	if (FGameplayEffectContext* BaseEffectContext = Handle.Get(); BaseEffectContext != nullptr && BaseEffectContext->GetScriptStruct()->IsChildOf(FTPSGameplayEffectContext::StaticStruct()))
	{
		return static_cast<FTPSGameplayEffectContext*>(BaseEffectContext);
	}
	
	return nullptr;
}

void FTPSGameplayEffectContext::SetAbilitySource(const ITPSAbilitySourceInterface* InObject, float InsSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
}

const ITPSAbilitySourceInterface* FTPSGameplayEffectContext::GetAbilitySource() const
{
	return Cast<ITPSAbilitySourceInterface>(AbilitySourceObject.Get());
}

FGameplayEffectContext* FTPSGameplayEffectContext::Duplicate() const
{
	FTPSGameplayEffectContext* NewContext = new FTPSGameplayEffectContext();
	*NewContext = *this;
	if (GetHitResult() != nullptr)
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}

UScriptStruct* FTPSGameplayEffectContext::GetScriptStruct() const
{
	return FTPSGameplayEffectContext::StaticStruct();
}

bool FTPSGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSucess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSucess);
	
	return true;
}

const UPhysicalMaterial* FTPSGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}
