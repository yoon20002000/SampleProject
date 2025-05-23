#include "AI/BTDecorator_TargetIsInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TPSAIController.h"
#include "Character/TPSCharacter.h"


UBTDecorator_TargetIsInRange::UBTDecorator_TargetIsInRange()
{
	NodeName = TEXT("Is In Range");
}

bool UBTDecorator_TargetIsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
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
	float AttackRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ATPSAIController::AIAttackRange);
	
	return Target->GetDistanceTo(ControllingPawn) <= AttackRange; 
}
