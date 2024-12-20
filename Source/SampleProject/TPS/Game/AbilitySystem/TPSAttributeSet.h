// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"
#include "TPSAttributeSet.generated.h"

struct FGameplayEffectSpec;
class UTPSAbilitySystemComponent;
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)			\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_SixParams(FTPSAttributeEvent,
	AActor* /*EffectInstigator*/,
	AActor* /*EffectCauser*/,
    const FGameplayEffectSpec* /*EffectSpec*/,
    float /*EffectMagnitude*/,
    float /*OldValue*/,
    float /*NewValue*/);
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UTPSAttributeSet();

	UWorld* GetWorld() const override;

	UTPSAbilitySystemComponent* GetTPSAbilitySystemComponent() const;
};
