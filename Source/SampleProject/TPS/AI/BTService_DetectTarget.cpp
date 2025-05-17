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
	
	if (ATPSPlayer* TargetPlayer = Cast<ATPSPlayer>(AINPC->GetHighestTarget()))
	{
		BBC->SetValueAsObject(ATPSAIController::TargetActorKey, TargetPlayer);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, .2f);
		DrawDebugPoint(World, TargetPlayer->GetActorLocation(), 10.0f, FColor::Blue, false, .2f);
		DrawDebugLine(World, AINPC->GetActorLocation(), TargetPlayer->GetActorLocation(), FColor::Blue, false, .2f);
	}
	else
	{
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, AINPC);
		bool bResult = World->OverlapMultiByChannel(OverlapResults,
													Center,
													FQuat::Identity,
													ECC_EngineTraceChannel2,
													FCollisionShape::MakeSphere(DetectRadius),
													CollisionQueryParam);
		
		if (bResult == true )
		{
			for (FOverlapResult Hit : OverlapResults)
			{
				TargetPlayer = Cast<ATPSPlayer>(Hit.GetActor());
				if (TargetPlayer != nullptr && TargetPlayer->GetController() != nullptr)
				{
					if (TargetPlayer->GetController()->IsPlayerController() == true && TargetPlayer->IsAlive() == true)
					{
						BBC->SetValueAsObject(ATPSAIController::TargetActorKey, TargetPlayer);
						DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, .2f);
						DrawDebugPoint(World, TargetPlayer->GetActorLocation(), 10.0f, FColor::Blue, false, .2f);
						DrawDebugLine(World, AINPC->GetActorLocation(), TargetPlayer->GetActorLocation(), FColor::Blue, false, .2f);
						return;	
					}
					else
					{
						BBC->SetValueAsObject(ATPSAIController::TargetActorKey, nullptr);
						break;
					}
				}
			}
			BBC->SetValueAsObject(ATPSAIController::TargetActorKey, nullptr);
			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Yellow, false, .2f);
		}
		else
		{
			BBC->SetValueAsObject(ATPSAIController::TargetActorKey, nullptr);
			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Yellow, false, .2f);
		}
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, .2f);
	}
}
