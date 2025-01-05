// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSBattleHUD.h"

#include "TPSHealthBar.h"

UTPSBattleHUD::UTPSBattleHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTPSBattleHUD::InitHealthBar(ATPSPlayer* Player)
{
	if (WBP_HealthBar == nullptr)
	{
		return;
	}
	WBP_HealthBar->InitHealthBar(Player);
}

void UTPSBattleHUD::UnitHealthBar()
{
	if (WBP_HealthBar == nullptr)
	{
		return;
	}
	WBP_HealthBar->UninitHealthBar();
}
