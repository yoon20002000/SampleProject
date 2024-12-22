// Fill out your copyright notice in the Description page of Project Settings.


#include "System/TPSAbilitySet.h"

#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"

void FTPSAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid() == true)
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FTPSAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid() == true)
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FTPSAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (Set == nullptr)
	{
		return;
	}

	GrantedAttributeSets.Add(Set);
}

void FTPSAbilitySet_GrantedHandles::ClearAbilitySystem(UTPSAbilitySystemComponent* TPSASComp)
{
	check(TPSASComp != nullptr);

	if (TPSASComp->IsOwnerActorAuthoritative() == false)
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid() == true)
		{
			TPSASComp->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid() == true)
		{
			TPSASComp->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		TPSASComp->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UTPSAbilitySet::UTPSAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTPSAbilitySet::GiveToAbilitySystem(UTPSAbilitySystemComponent* TPSASComp,
	FTPSAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(TPSASComp != nullptr);

	if (TPSASComp->IsOwnerActorAuthoritative() == false)
	{
		return;
	}

	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FTPSAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];
		if (IsValid(SetToGrant.AttributeSet) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Granted Attributes [%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(TPSASComp->GetOwner(), SetToGrant.AttributeSet);
		TPSASComp->AddAttributeSetSubobject(NewSet);
		
		if (OutGrantedHandles != nullptr)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
	
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FTPSAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (IsValid(AbilityToGrant.Ability) == false)
		{
			UE_LOG(LogTemp, Log, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid"), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UTPSGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UTPSGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = TPSASComp->GiveAbility(AbilitySpec);

		if (OutGrantedHandles !=nullptr)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	// {
	// 	const FTPSAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];
	//
	// 	if (IsValid(EffectToGrant.GameplayEffect) == false)
	// 	{
	// 		continue;
	// 	}
	//
	// 	const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
	// 	const FActiveGameplayEffectHandle GameplayEffectHandle = TPSASComp->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, TPSASComp->MakeEffectContext());
	//
	// 	if (OutGrantedHandles != nullptr)
	// 	{
	// 		OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
	// 	}
	// }
}
