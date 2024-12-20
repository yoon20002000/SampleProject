// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayEffectTypes.h"


#include "TPSGameplayEffectContext.generated.h"

class ITPSAbilitySourceInterface;
struct FGameplayEffectContextHandle;

USTRUCT()
struct FTPSGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	FTPSGameplayEffectContext();
	FTPSGameplayEffectContext(AActor* Instigator, AActor* InEffectCauser);

	static SAMPLEPROJECT_API FTPSGameplayEffectContext* ExtractEffectContext(FGameplayEffectContextHandle Handle);

	void SetAbilitySource(const ITPSAbilitySourceInterface* InObject, float InsSourceLevel);

	const ITPSAbilitySourceInterface* GetAbilitySource() const;
	virtual FGameplayEffectContext* Duplicate() const override;
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSucess) override;
	const UPhysicalMaterial* GetPhysicalMaterial() const;

public:
	UPROPERTY()
	int32 CartridgeID = -1;

protected:
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FTPSGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FTPSGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
