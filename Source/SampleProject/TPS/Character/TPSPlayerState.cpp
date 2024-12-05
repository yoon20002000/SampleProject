// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSPlayerState.h"

ATPSPlayerState::ATPSPlayerState() : KillCount(0)
{
}

void ATPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	ClearKillCount();
}

void ATPSPlayerState::AddKillCount()
{
	UpdateKillCount(KillCount + 1);
}

void ATPSPlayerState::ClearKillCount()
{
	UpdateKillCount(0);
}

void ATPSPlayerState::UpdateKillCount(uint32 InKillCount)
{
	uint32 OldKillCount = KillCount;
	KillCount = InKillCount;
	uint32 Delta = KillCount - OldKillCount;

	OnKillCountChanged.Broadcast(this, KillCount, Delta);
}
