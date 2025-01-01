// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

class UTPSGA_Attack;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Attack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual auto TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) -> void override;

private:
	FDelegateHandle AttackEndDelegateHandle;
	bool IsAttacking;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPSGA_Attack> GA_Attack;
};
