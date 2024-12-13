// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Game/TPSGameplayAbility.h"
#include "TPSAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UTPSAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();
	
protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	int32 ActivationGroupCounts[static_cast<uint8>(ETPSAbilityActivationGroup::MAX)];
};
