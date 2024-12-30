// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UBTTask_FindPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FindPatrolLocation();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
