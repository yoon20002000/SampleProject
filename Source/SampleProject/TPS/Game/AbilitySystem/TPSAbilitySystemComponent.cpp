// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"

#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

UTPSAbilitySystemComponent::UTPSAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	InputHeldSpecHandles.Reset();
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();

	FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

void UTPSAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == true)
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability != nullptr && AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) == true)
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UTPSAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == true)
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
			InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
		}
	}
}

void UTPSAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked) == true)
	{
		ClearAbilityInput();
	}
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle))
		{
			if (AbilitySpec->Ability != nullptr && AbilitySpec->IsActive() == false)
			{
				const UTPSGameplayAbility* TPSAbilityCDO = Cast<UTPSGameplayAbility>(AbilitySpec->Ability);
				if (TPSAbilityCDO !=nullptr && TPSAbilityCDO->GetActivationPolicy() == ETPSAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle))
		{
			if (AbilitySpec->Ability != nullptr)
			{
				AbilitySpec->InputPressed = true;
				if (AbilitySpec->IsActive() == true)
				{
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UTPSGameplayAbility* TPSAbilityCDO = Cast<UTPSGameplayAbility>(AbilitySpec->Ability);
					if (TPSAbilityCDO != nullptr && TPSAbilityCDO->GetActivationPolicy() == ETPSAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle))
		{
			if (AbilitySpec->Ability == true)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive() == true)
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UTPSAbilitySystemComponent::ClearAbilityInput()
{
	InputHeldSpecHandles.Reset();
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
