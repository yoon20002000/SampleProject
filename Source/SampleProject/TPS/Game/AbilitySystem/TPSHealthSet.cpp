// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSHealthSet.h"
#include "GameplayEffectExtension.h"
#include "TPSAbilitySystemComponent.h"
#include "Game/TPSGameplayTags.h"
#include "Net/UnrealNetwork.h"

UTPSHealthSet::UTPSHealthSet() :
		Health(100.0f),
		MaxHealth(120.0f),
		bOutOfHealth(false),
		MaxHealthBeforeAttributeChange(0.0f),
		HealthBeforeAttributeChange(0.0f)
{
}

void UTPSHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTPSHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTPSHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UTPSHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSHealthSet, Health, OldValue);

	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();
	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);

	if (bOutOfHealth == false && CurrentHealth <= 0.0f)
	{
		OnOutOfHealth.Broadcast(nullptr,nullptr,nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
	}

	bOutOfHealth = CurrentHealth <= 0.0f;
}

void UTPSHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTPSHealthSet, MaxHealth, OldValue);

	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxHealth() - OldValue.GetCurrentValue(),
	                             OldValue.GetCurrentValue(), GetMaxHealth());
}
bool UTPSHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Super::PreGameplayEffectExecute(Data) == false)
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			const bool bIsDamageFromSelfDestruct = Data.EffectSpec.GetDynamicAssetTags().HasTagExact(TPSGameplayTags::Gameplay_DamageSelfDestruct);

			if (Data.Target.HasMatchingGameplayTag(TPSGameplayTags::Gameplay_DamageImmunity) && bIsDamageFromSelfDestruct == false)
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}

	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();
	
	return true;
}

void UTPSHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const bool bIsDamageFromSelfDestruct = Data.EffectSpec.GetDynamicAssetTags().HasTagExact(TPSGameplayTags::Gameplay_DamageSelfDestruct);
	float MinHealth = 0.0f;

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), MinHealth, GetMaxHealth()));
		SetHealing(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinHealth, GetMaxHealth()));
		OnMaxHealthChanged.Broadcast(
			Instigator, 
			Causer, 
			&Data.EffectSpec, 
			Data.EvaluatedData.Magnitude, 
			MaxHealthBeforeAttributeChange, 
			GetMaxHealth()
		);
	}
	
	if (GetHealth() != HealthBeforeAttributeChange)
	{
		OnHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, GetHealth(), HealthBeforeAttributeChange, GetMaxHealth());
	}

	if (GetHealth() <= 0 && bOutOfHealth == false)
	{
		OnOutOfHealth.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, MaxHealthBeforeAttributeChange, GetMaxHealth());
	}

	bOutOfHealth = GetHealth() <= 0.0f;
}

void UTPSHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UTPSHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UTPSHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > NewValue)
		{
			UTPSAbilitySystemComponent* ASC = GetTPSAbilitySystemComponent();
			check(ASC != nullptr);

			ASC->ApplyModToAttribute(GetHealthAttribute(),EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfHealth == true && GetHealth() > 0.0f)
	{
		bOutOfHealth = false;
	}
}

void UTPSHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue,1.0f);
	}
}
