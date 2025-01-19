// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToDistance.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UBTTask_MoveToDistance : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	UBTTask_MoveToDistance();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere)
	float StopDistance;
};
