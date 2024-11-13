// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"

#include "TPSGameInstance.h"
#include "TPSGameManager.h"
#include "TPSSystemManager.h"

void ATPSGameMode::StartPlay()
{
	Super::StartPlay();

	UTPSSystemManager::Get()->SetWorld(GetWorld());

	UTPSSystemManager::Get()->GetGameManager()->InitData(this); 
	
	UTPSSystemManager::Get()->BeginPlay();
}

void ATPSGameMode::BeginDestroy()
{	
	Super::BeginDestroy();
}

void ATPSGameMode::Tick(float DeltaSeconds)
{
	UTPSSystemManager::Get()->Tick(DeltaSeconds);
	Super::Tick(DeltaSeconds);
	UTPSSystemManager::Get()->PostTick(DeltaSeconds);
}

void ATPSGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UTPSSystemManager::Get()->EndPlay(EndPlayReason);
	Super::EndPlay(EndPlayReason);
}
