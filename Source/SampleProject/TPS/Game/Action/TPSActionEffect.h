// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Action/TPSAction.h"
#include "TPSActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSActionEffect : public UTPSAction
{
	GENERATED_BODY()

public:
	UTPSActionEffect();
	float GetTimeRemaining() const;

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;
};
