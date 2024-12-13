// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Input/TPSInputConfig.h"

UTPSInputConfig::UTPSInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UTPSInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTPSInputAction Action : NativeInputActions)
	{
		if (Action.InputAction != nullptr && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Can't find input action for %s. [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}

const UInputAction* UTPSInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTPSInputAction Action : AbilityInputActions)
	{
		if (Action.InputAction != nullptr && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Can't find input action for %s. [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	
	return nullptr;
}
