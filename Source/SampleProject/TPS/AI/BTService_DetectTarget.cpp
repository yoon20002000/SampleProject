// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_DetectTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TPSAIController.h"
#include "Character/TPSNonPlayer.h"
#include "Character/TPSPlayer.h"
#include "Engine/OverlapResult.h"


UBTService_DetectTarget::UBTService_DetectTarget() : DetectRadius(1000.0f)
{
	NodeName = TEXT("Detect Target");
	Interval = 1.0f;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	ATPSNonPlayer* AINPC = Cast<ATPSNonPlayer>(OwnerComp.GetAIOwner()->GetPawn());
	if (AINPC == nullptr)
	{
		return;
	}

	UWorld* World = AINPC->GetWorld();
	FVector Center = AINPC->GetActorLocation();
	
	if (World == nullptr)
	{
		return;
	}
	
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	if (BBC == nullptr)
	{
		return;
	}
	AActor* TargetPlayer = FindTarget(AINPC, World, Center);
	BBC->SetValueAsObject(ATPSAIController::TargetActorKey, TargetPlayer);
	
	if (TargetPlayer != nullptr)
	{
		DrawDebugLine(World, AINPC->GetActorLocation(), TargetPlayer->GetActorLocation(), FColor::Blue, false, .2f);
		DrawDebugPoint(World, TargetPlayer->GetActorLocation(), 10.0f, FColor::Blue, false, .2f);
	}
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, .2f);
}

AActor* UBTService_DetectTarget::FindTarget(ATPSNonPlayer* InAINPC, UWorld* InWorld, const FVector& InCenter) const
{
	if (ATPSPlayer* ThreatTarget = Cast<ATPSPlayer>(InAINPC->GetHighestTarget()))
	{
		return ThreatTarget;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, InAINPC);
	bool bResult = InWorld->OverlapMultiByChannel(OverlapResults,
												InCenter,
												FQuat::Identity,
												ECC_EngineTraceChannel2,
												FCollisionShape::MakeSphere(DetectRadius),
												CollisionQueryParam);
		
	if (bResult == true )
	{
		for (FOverlapResult Hit : OverlapResults)
		{
			if (ATPSPlayer* ThreatTarget = Cast<ATPSPlayer>(Hit.GetActor()))
			{
				if (ThreatTarget->IsAlive() && ThreatTarget->GetController() && ThreatTarget->GetController()->IsPlayerController())
				{
					return ThreatTarget;
				}
			}
		}
		return nullptr;
	}
	else
	{
		return nullptr;
	}
		
}
