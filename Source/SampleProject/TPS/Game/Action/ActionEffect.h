// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Action/Action.h"
#include "ActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UActionEffect : public UAction
{
	GENERATED_BODY()

public:
	UActionEffect();
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
