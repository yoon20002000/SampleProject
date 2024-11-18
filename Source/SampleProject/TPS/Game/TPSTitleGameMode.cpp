// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TPSTitleGameMode.h"

#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "UI/TPSUIManager.h"

void ATPSTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	UTPSSystemManager::Get()->GetGameManager()->SpawnPlayer(TEXT("TitlePlayer"));
}
