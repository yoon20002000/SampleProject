// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"

#include "TPSGameManager.h"
#include "TPSSystemManager.h"
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

void ATPSGameMode::OnActorKilled(AActor* Causer, AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Causer Actor : %s Actor is Death : %s"), *GetNameSafe(Causer),
	       *InstigatorActor->GetName());
	
	if (ATPSPlayer* TPSPlayer = Cast<ATPSPlayer>(InstigatorActor))
	{
		if(ATPSPlayerState* PS = Cast<ATPSPlayerState>(TPSPlayer->GetPlayerState()))
		{
			PS->AddDeathCount();
		}
	}
}
