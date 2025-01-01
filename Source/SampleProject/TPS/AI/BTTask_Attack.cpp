// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "Character/TPSCharacter.h"


UBTTask_Attack::UBTTask_Attack() : IsAttacking(false)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);;

	ATPSCharacter* Character = Cast<ATPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Character-> 공격 action call 필요
	UE_LOG(LogTemp, Log, TEXT("Action Call 필요"));
	IsAttacking = true;
	if (AttackEndDelegateHandle.IsValid() == false)
	{
		Character->OnAttackEnd.AddLambda(
			[this]() -> void
			{
				IsAttacking = false;
			});	
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (IsAttacking == false)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	}
}
