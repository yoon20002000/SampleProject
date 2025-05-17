// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TargetIsInRange.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UBTDecorator_TargetIsInRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_TargetIsInRange();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
