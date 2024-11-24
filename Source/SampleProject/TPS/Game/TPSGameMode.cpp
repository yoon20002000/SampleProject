// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"

#include "TPSGameManager.h"
#include "TPSSystemManager.h"

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

void ATPSGameMode::OnActorKilled(AActor* Actor, AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("Actor is Death : %s"), *InstigatorActor->GetName());
}
