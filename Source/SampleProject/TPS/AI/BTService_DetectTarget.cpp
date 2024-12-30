// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_DetectTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/TPSAIController.h"
#include "Character/TPSPlayer.h"
#include "Engine/OverlapResult.h"


UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("Detect Target");
	Interval = 1.0f;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn == nullptr)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (World == nullptr)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(OverlapResults,
	                                            Center,
	                                            FQuat::Identity,
	                                            ECC_EngineTraceChannel2,
	                                            FCollisionShape::MakeSphere(DetectRadius),
	                                            CollisionQueryParam);

	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	if (bResult == true)
	{
		for (FOverlapResult Hit : OverlapResults)
		{
			ATPSPlayer* TargetPlayer = Cast<ATPSPlayer>(Hit.GetActor());
			if (TargetPlayer != nullptr && TargetPlayer->GetController()->IsPlayerController() == true)
			{
				BBC->SetValueAsObject(ATPSAIController::TargetActor, TargetPlayer);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, .2f);
				DrawDebugPoint(World, TargetPlayer->GetActorLocation(), 10.0f, FColor::Blue, false, .2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), TargetPlayer->GetActorLocation(), FColor::Blue, false, .2f);
				return;
			}
		}
		BBC->SetValueAsObject(ATPSAIController::TargetActor, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Yellow, false, .2f);
	}
	else
	{
		BBC->SetValueAsObject(ATPSAIController::TargetActor, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Yellow, false, .2f);
	}
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, .2f);
}
