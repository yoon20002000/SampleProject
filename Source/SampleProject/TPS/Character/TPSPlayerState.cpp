// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSPlayerState.h"

ATPSPlayerState::ATPSPlayerState() : DeathCount(0)
{
}

void ATPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	ClearDeathCount();
}

void ATPSPlayerState::AddDeathCount()
{
	++DeathCount;
}

void ATPSPlayerState::ClearDeathCount()
{
	DeathCount = 0;
}
