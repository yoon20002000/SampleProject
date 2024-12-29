// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAIController.h"

#include "TPSPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

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
