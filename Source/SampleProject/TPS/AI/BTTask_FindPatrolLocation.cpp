// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TPSAIController.h"


UBTTask_FindPatrolLocation::UBTTask_FindPatrolLocation()
{
	NodeName = TEXT("Find Patrol Location");
}

EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());

	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	auto BBC = OwnerComp.GetBlackboardComponent();
	
	FVector Origin = BBC->GetValueAsVector(ATPSAIController::StartLocationKey);
	FNavLocation NextPatrolLoc;

	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextPatrolLoc))
	{
		BBC->SetValueAsVector(ATPSAIController::PatrolLocationKey, NextPatrolLoc);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed; 
}
