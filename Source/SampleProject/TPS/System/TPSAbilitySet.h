// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TPSAbilitySet.generated.h"

class UTPSAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class UAttributeSet;
class UTPSGameplayAbility;

USTRUCT(BlueprintType)
struct FTPSAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTPSGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, meta = (Categoreis = "InputTag"))
	FGameplayTag InputTag;
};

// USTRUCT(BlueprintType)
// struct FTPSAbilitySet_GameplayEffect
// {
// 	GENERATED_BODY()
// public:
// 	UPROPERTY(EditDefaultsOnly)
// 	
// };
USTRUCT(BlueprintType)
struct FTPSAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};

USTRUCT(BlueprintType)
struct FTPSAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void ClearAbilitySystem(UTPSAbilitySystemComponent* TPSASComp);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/**
 * 
 */
UCLASS(BlueprintType, Const)
class SAMPLEPROJECT_API UTPSAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UTPSAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UTPSAbilitySystemComponent* TPSASComp, FTPSAbilitySet_GrantedHandles* OutGrantedHandles,
	                         UObject* SourceObject = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = "Ability"))
	TArray<FTPSAbilitySet_GameplayAbility> GrantedGameplayAbilities;
	// UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = "GameplayEffect"))
	// TArray<FTPSAbilitySet_GameplayEffect> GrantedGameplayEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Abilitie Sets", meta=(TitleProperty = "AbilitySet"))
	TArray<FTPSAbilitySet_AttributeSet> GrantedAttributes;
};
