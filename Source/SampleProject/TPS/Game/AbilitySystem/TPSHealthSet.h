// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "Game/AbilitySystem/TPSAttributeSet.h"
#include "TPSHealthSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SAMPLEPROJECT_API UTPSHealthSet : public UTPSAttributeSet
{
	GENERATED_BODY()
public:
	UTPSHealthSet();

	ATTRIBUTE_ACCESSORS(UTPSHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UTPSHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UTPSHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UTPSHealthSet, Damage);

	mutable FTPSAttributeEvent OnHealthChanged;
	mutable FTPSAttributeEvent OnMaxHealthChanged;
	mutable FTPSAttributeEvent OnOutOfHealth;
	
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "TPS|Health", meta=(HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "TPS|Health", meta=(HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	bool bOutOfHealth;

	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	UPROPERTY(BlueprintReadOnly, Category = "TPS|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;
	UPROPERTY(BlueprintReadOnly, Category = "TPS|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
};
