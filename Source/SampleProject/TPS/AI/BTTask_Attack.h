// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

class ATPSCharacter;
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

private:
	TWeakObjectPtr<ATPSCharacter> Character;
	FDelegateHandle AttackEndDelegateHandle;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTPSGA_Attack> GA_Attack;
};
