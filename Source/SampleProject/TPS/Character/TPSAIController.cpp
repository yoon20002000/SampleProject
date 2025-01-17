// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ATPSAIController::StartLocationKey(TEXT("StartLocation"));
const FName ATPSAIController::PatrolLocationKey(TEXT("PatrolLocation"));
const FName ATPSAIController::TargetActorKey(TEXT("TargetActor"));

ATPSAIController::ATPSAIController()
{
	
}

void ATPSAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATPSAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATPSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BTAsset == nullptr || BBAsset == nullptr)
	{
		return;
	}
	RunAI(InPawn);
}

void ATPSAIController::RunAI(const AActor* ThisActor)
{
	UBlackboardComponent* BBC = Blackboard;
	if (UseBlackboard(BBAsset, BBC))
	{
		BBC->SetValueAsVector(StartLocationKey, ThisActor->GetActorLocation());
		if (RunBehaviorTree(BTAsset) == false)
		{
			UE_LOG(LogTemp, Log, TEXT("AIController not run"));
		}
	}
}


void ATPSAIController::StopAI() const
{
	UBehaviorTreeComponent* BTC = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTC != nullptr)
	{
		BTC->StopTree(EBTStopMode::Safe);
	}
}
