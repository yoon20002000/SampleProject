// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TPSGameplayAbility.h"

#include "AbilitySystem/TPSAbilitySourceInterface.h"
#include "AbilitySystem/TPSGameplayEffectContext.h"

UTPSGameplayAbility::UTPSGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ActivationPolicy = ETPSAbilityActivationPolicy::OnInputTriggered;
	ActivationGroup = ETPSAbilityActivationGroup::Independent;
}

ETPSAbilityActivationPolicy UTPSGameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}

ETPSAbilityActivationGroup UTPSGameplayAbility::GetActivationGroup() const
{
	return ActivationGroup;
}

FGameplayEffectContextHandle UTPSGameplayAbility::MakeEffectContext(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo) const
{
	FGameplayEffectContextHandle ContextHandle = Super::MakeEffectContext(Handle, ActorInfo);

	FTPSGameplayEffectContext* EffectContext = FTPSGameplayEffectContext::ExtractEffectContext(ContextHandle);
	check(EffectContext);
	
	check(ActorInfo);

	AActor* EffectCauser = nullptr;
	const ITPSAbilitySourceInterface* AbilitySource = nullptr;
	float SourceLevel = 0.0f;

	GetAbilitySource(Handle, ActorInfo, SourceLevel, AbilitySource, EffectCauser);

	UObject* SourceObject = GetSourceObject(Handle, ActorInfo);

	AActor* Instigator = ActorInfo != nullptr ? ActorInfo->OwnerActor.Get() : nullptr;

	EffectContext->SetAbilitySource(AbilitySource, SourceLevel);
	EffectContext->AddInstigator(Instigator, EffectCauser);
	EffectContext->AddSourceObject(SourceObject);
	
	return ContextHandle;
}

void UTPSGameplayAbility::GetAbilitySource(FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, float& OutSourceLevel,
	const ITPSAbilitySourceInterface*& OutAbilitySource, AActor*& OutEffectCauser) const
{
	OutSourceLevel = 0.0f;
	OutAbilitySource = nullptr;
	OutEffectCauser = nullptr;

	OutEffectCauser = ActorInfo->AvatarActor.Get();

	UObject* SourceObject = GetSourceObject(Handle, ActorInfo);

	OutAbilitySource = Cast<ITPSAbilitySourceInterface>(SourceObject);
}
