// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "TPSGameplayAbility.generated.h"

class ITPSAbilitySourceInterface;

UENUM(BlueprintType)
enum class ETPSAbilityActivationPolicy : uint8
{
	OnInputTriggered,

	WhileInputActive,

	OnSpawn
};

UENUM(BlueprintType)
enum class ETPSAbilityActivationGroup : uint8
{
	Independent,
	ExclusiveReplaceable,
	ExclusiveBlocking,

	MAX UMETA(Hidden)
};
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UTPSAbilitySystemComponent;
public:
	UTPSGameplayAbility(const FObjectInitializer& ObjectInitializer);

	ETPSAbilityActivationPolicy GetActivationPolicy() const ;
	ETPSAbilityActivationGroup GetActivationGroup() const ;
protected:
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;

	virtual void GetAbilitySource(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel, const ITPSAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser)const;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "TPS|Ability Activation")
	ETPSAbilityActivationPolicy ActivationPolicy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "TPS|Ability Activation")
	ETPSAbilityActivationGroup ActivationGroup;
};
