// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSBattleHUD.h"

#include "TPSCooldown.h"
#include "TPSHealthBar.h"
#include "UIKillCounter.h"
#include "Character/TPSCharacter.h"
#include "Character/TPSPlayer.h"

UTPSBattleHUD::UTPSBattleHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTPSBattleHUD::InitBattleHUD(ATPSPlayer* Player)
{
	InitHealthBar(Player);
	InitKillCounter(Player);
	InitCooldown(Player);
}

void UTPSBattleHUD::UninitBattleHUD(ATPSPlayer* Player)
{
	UninitHealthBar(Player);
	UninitKillCounter(Player);
	UninitCooldown(Player);
}

void UTPSBattleHUD::InitHealthBar(ATPSPlayer* Player)
{
	if (WBP_HealthBar == nullptr)
	{
		return;
	}
	WBP_HealthBar->InitHealthBar(Player);
}

void UTPSBattleHUD::UninitHealthBar(ATPSPlayer* Player)
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

void UTPSBattleHUD::UninitKillCounter(ATPSPlayer* Player)
{
	if (WBP_DeathCount == nullptr)
	{
		return;
	}
	WBP_DeathCount->UninitKillCount(Player);
}

void UTPSBattleHUD::InitCooldown(ATPSPlayer* Player)
{
	if (WBP_Cooldown == nullptr)
	{
		return;
	}
	if (ATPSCharacter* Character = Cast<ATPSCharacter>(Player))
	{
		WBP_Cooldown->InitCooldownUI(Character);	
	}
}

void UTPSBattleHUD::UninitCooldown(ATPSPlayer* Player)
{
	if (WBP_Cooldown == nullptr)
	{
		return;
	}
	if (ATPSCharacter* Character = Cast<ATPSCharacter>(Player))
	{
		WBP_Cooldown->UninitCooldownUI(Character);	
	}
}
