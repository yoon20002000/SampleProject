// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAIController.h"

#include "NavigationSystem.h"
#include "TPSPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"

ATPSAIController::ATPSAIController() : RepeatInterval(3.0f)
{
	
}

void ATPSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ThisClass::OnRepeatTimer, RepeatInterval,
	                                       true);
}

void ATPSAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void ATPSAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool ATPSAIController::IsAlive() const
{
	ATPSCharacter* ControlledCharacter = Cast<ATPSCharacter>(GetPawn());
	if (UTPSHealthComponent* HealthComp = ControlledCharacter->GetHealthAttributeComp())
	{
		return HealthComp->IsAlive();
	}
	return false;
}

void ATPSAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBT != nullptr)
	{
		RunBehaviorTree(AIBT);

		if (AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(),0))
		{
			//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerActor->GetActorLocation());
			GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		}
	}
}

void ATPSAIController::OnRepeatTimer()
{
	APawn* CurrentPawn = GetPawn();
	check(CurrentPawn);

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (NavSystem == nullptr)
	{
		return;
	}

	FNavLocation NextLocation ;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation);
	}
		
}
