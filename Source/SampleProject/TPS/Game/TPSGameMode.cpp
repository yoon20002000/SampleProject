// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"

#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "Character/TPSAIController.h"
#include "Character/TPSPlayer.h"
#include "Character/TPSPlayerState.h"

ATPSGameMode::ATPSGameMode()
{
}

void ATPSGameMode::StartPlay()
{
	Super::StartPlay();

	UTPSSystemManager::Get()->SetWorld(GetWorld());

	UTPSSystemManager::Get()->GetGameManager()->InitData(this); 
	
	UTPSSystemManager::Get()->BeginPlay();

	UTPSSystemManager::Get()->SetGameState(GameplayState);
}

void ATPSGameMode::BeginDestroy()
{	
	Super::BeginDestroy();
	
}

void ATPSGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UTPSSystemManager::Get()->EndPlay(EndPlayReason);
	Super::EndPlay(EndPlayReason);
}

void ATPSGameMode::OnActorKilled(AActor* KilledActor, AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("KilledActor Actor : %s InstigatorActor : %s"), *GetNameSafe(KilledActor),
	       *InstigatorActor->GetName());
	
	if (const ATPSPlayer* TPSPlayer = Cast<ATPSPlayer>(KilledActor))
	{
		ATPSCharacter* AICharacter = Cast<ATPSCharacter>(InstigatorActor);
		if (AICharacter != nullptr)
		{
			if (ATPSAIController* AIController = Cast<ATPSAIController>(AICharacter->GetController()))
			{
				AIController->StopAI();
			}
		}
	}
	else
	{
		TPSPlayer = Cast<ATPSPlayer>(InstigatorActor); 
		
		if(ATPSPlayerState* PS = Cast<ATPSPlayerState>(TPSPlayer->GetPlayerState()))
		{
			PS->AddKillCount();
		}
	}
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(
		[KilledActor, this]()
		{
			ATPSCharacter* DieCharacter = Cast<ATPSCharacter>(KilledActor);
			ATPSPlayer* Player = Cast<ATPSPlayer>(DieCharacter);
			
			UClass* SpawnActor = Player != nullptr ? PlayerClass : AICharacterClass;
			
			FVector SpawnLocation(0.0f, 0.0f, 100.0f);
			FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
			APawn* SpawnedActor = GetWorld()->SpawnActor<APawn>(SpawnActor, SpawnLocation, SpawnRotation);
			
			if (SpawnedActor != nullptr && Player != nullptr)
			{
				if (APlayerController* PC = Cast<APlayerController>(Player->GetController()) )
				{
					PC->UnPossess();
					PC->Possess(SpawnedActor);	
				}
			}
			DieCharacter->UninitAndDestroy();
		});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,5,false);
}
