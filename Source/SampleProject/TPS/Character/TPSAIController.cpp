// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

const FName ATPSAIController::StartLocation(TEXT("StartLocation"));
const FName ATPSAIController::PatrolLocation(TEXT("PatrolLocation"));
const FName ATPSAIController::TargetActor(TEXT("TargetActor"));

ATPSAIController::ATPSAIController() 
{
	
}

void ATPSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BTAsset==nullptr || BBAsset==nullptr)
	{
		return;
	}
	UBlackboardComponent* BBC = Blackboard;
	if (UseBlackboard(BBAsset,BBC))
	{
		BBC->SetValueAsVector(StartLocation, InPawn->GetActorLocation());
		
		if (RunBehaviorTree(BTAsset) == false)
		{
			UE_LOG(LogTemp, Log, TEXT("AIController not run"));
		}
	}
}
