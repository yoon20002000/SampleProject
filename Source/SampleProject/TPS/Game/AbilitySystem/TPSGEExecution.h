#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "TPSGEExecution.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGEExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UTPSGEExecution();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
