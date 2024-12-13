// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "TPSInputConfig.h"
#include "TPSEnhancedInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UTPSInputConfig;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UTPSEnhancedInputComponent(const FObjectInitializer& ObjectInitializer);

	template <class UserClass, typename FuncType>
	void BindNativeAction(const UTPSInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent,
	                      UserClass* Object, FuncType Func);
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UTPSInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                        ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	void RemoveBinds(TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
void UTPSEnhancedInputComponent::BindNativeAction(const UTPSInputConfig* InputConfig, const FGameplayTag& InputTag,
                                                  ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);

	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UTPSEnhancedInputComponent::BindAbilityActions(const UTPSInputConfig* InputConfig, UserClass* Object,
                                                    PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                                    TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FTPSInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction != nullptr && Action.InputTag.IsValid())
		{
			if (PressedFunc != nullptr)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc,
				                           Action.InputTag).GetHandle());
			}

			if (ReleasedFunc != nullptr)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc,
				                           Action.InputTag).GetHandle());
			}
		}
	}
}
