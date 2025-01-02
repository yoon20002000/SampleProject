// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TPSAIController.h"
#include "Character/TPSCharacter.h"


UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	bNotifyTick = false;
	NodeName =TEXT("Turn To Target");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATPSCharacter* TPSChar = Cast<ATPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (TPSChar == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ATPSCharacter* Target = Cast<ATPSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATPSAIController::TargetActorKey));
	if (Target == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - TPSChar->GetActorLocation();
	LookVector.Z = 0;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	TPSChar->SetActorRotation(FMath::RInterpTo(TPSChar->GetActorRotation(), TargetRot,GetWorld()->DeltaTimeSeconds,2.f));
	return EBTNodeResult::Succeeded;
}
