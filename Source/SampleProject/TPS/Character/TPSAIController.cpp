// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAIController.h"

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
		if (RunBehaviorTree(BTAsset) == false)
		{
			UE_LOG(LogTemp, Log, TEXT("AIController not run"));
		}
	}
}
