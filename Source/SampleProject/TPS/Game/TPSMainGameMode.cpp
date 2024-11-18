// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TPSMainGameMode.h"

#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "UI/TPSUIManager.h"

void ATPSMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	UTPSSystemManager::Get()->GetGameManager()->SpawnPlayer(TEXT("Player"));
}
