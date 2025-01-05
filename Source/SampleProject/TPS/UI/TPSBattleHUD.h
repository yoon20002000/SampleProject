// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/TPSHUDLayout.h"
#include "TPSBattleHUD.generated.h"

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
	void InitHealthBar(ATPSPlayer* Player);
	void UnitHealthBar();
private:
	UPROPERTY(EditAnywhere, Category=UI, meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTPSHealthBar> WBP_HealthBar;
};
