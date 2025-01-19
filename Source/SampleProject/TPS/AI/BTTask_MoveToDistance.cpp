// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToDistance.h"

#include "AIController.h"
#include "Character/TPSAIController.h"


UBTTask_MoveToDistance::UBTTask_MoveToDistance() : StopDistance(0.0f)
{
	NodeName = TEXT("Move To Distance");
	bNotifyTick = true;
}

void UBTTask_MoveToDistance::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return;
	}
	
	APawn* ControlledPawn = AIController->GetPawn();
	if (ControlledPawn == nullptr)
	{
		return;
	}
	
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
	if (BBC == nullptr)
	{
		return;
	}
	
	APawn* Target = Cast<APawn>(BBC->GetValueAsObject(ATPSAIController::TargetActorKey));
	if (Target == nullptr)
	{
		return;
	}
	
	if (Target->GetDistanceTo(ControlledPawn) <= StopDistance)
	{
		AIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
