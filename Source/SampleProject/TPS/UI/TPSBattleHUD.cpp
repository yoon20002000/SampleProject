// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSBattleHUD.h"

#include "TPSHealthBar.h"
#include "UIKillCounter.h"

UTPSBattleHUD::UTPSBattleHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTPSBattleHUD::InitBattleHUD(ATPSPlayer* Player)
{
	InitHealthBar(Player);
	InitKillCounter(Player);
}

void UTPSBattleHUD::UninitBattleHUD(ATPSPlayer* Player)
{
	UnitHealthBar(Player);
	UnitKillCounter(Player);
}

void UTPSBattleHUD::InitHealthBar(ATPSPlayer* Player)
{
	if (WBP_HealthBar == nullptr)
	{
		return;
	}
	WBP_HealthBar->InitHealthBar(Player);
}

void UTPSBattleHUD::UnitHealthBar(ATPSPlayer* Player)
{
	if (WBP_HealthBar == nullptr)
	{
		return;
	}
	WBP_HealthBar->UninitHealthBar(Player);
}

void UTPSBattleHUD::InitKillCounter(ATPSPlayer* Player)
{
	if (WBP_DeathCount == nullptr)
	{
		return;
	}
	WBP_DeathCount->InitKillCount(Player);
}

void UTPSBattleHUD::UnitKillCounter(ATPSPlayer* Player)
{
	if (WBP_DeathCount == nullptr)
	{
		return;
	}
	WBP_DeathCount->UninitKillCount(Player);
}
