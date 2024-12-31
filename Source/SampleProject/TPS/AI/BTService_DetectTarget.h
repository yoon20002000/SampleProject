// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "BTService_DetectTarget.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UBTService_DetectTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_DetectTarget();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere)
	float DetectRadius = 600.0f;
};
