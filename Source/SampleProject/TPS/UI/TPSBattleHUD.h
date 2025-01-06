// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/TPSHUDLayout.h"
#include "TPSBattleHUD.generated.h"

class UUIKillCounter;
class ATPSPlayer;
class UTPSHealthBar;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSBattleHUD : public UTPSHUDLayout
{
	GENERATED_BODY()

public:
	UTPSBattleHUD(const FObjectInitializer& ObjectInitializer);
	void InitBattleHUD(ATPSPlayer* Player);
	void UninitBattleHUD(ATPSPlayer* Player);
	
private:
	void InitHealthBar(ATPSPlayer* Player);
	void UninitHealthBar(ATPSPlayer* Player);
	void InitKillCounter(ATPSPlayer* Player);
	void UninitKillCounter(ATPSPlayer* Player);
private:
	UPROPERTY(EditAnywhere, Category=UI, meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTPSHealthBar> WBP_HealthBar;
	UPROPERTY(EditAnywhere, Category=UI, meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UUIKillCounter> WBP_DeathCount;
};
