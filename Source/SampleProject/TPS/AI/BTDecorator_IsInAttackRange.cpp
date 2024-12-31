// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsInAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TPSAIController.h"
#include "Character/TPSCharacter.h"


UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange() : AttackRangeDistance(0)
{
    NodeName = TEXT("Can Attack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
	ATPSCharacter* Target = Cast<ATPSCharacter>(BBC->GetValueAsObject(ATPSAIController::TargetActorKey));
	if (Target == nullptr)
	{
		return false;
	}

	bResult = Target->GetDistanceTo(ControllingPawn) <= AttackRangeDistance;
	return bResult;
}
