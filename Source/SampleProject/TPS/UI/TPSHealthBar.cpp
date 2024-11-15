// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSHealthBar.h"

#include "TPSHelper.h"
#include "Character/TPSPlayer.h"
#include "Components/ProgressBar.h"

bool UTPSHealthBar::Initialize()
{
	bool bResult = Super::Initialize();
	// 캐릭터 체력 바 기본 세팅

	if (APlayerController* PlayerController = TPSHelper::GetPlayerController())
	{
		if (ATPSPlayer* TPSPlayer = CastChecked<ATPSPlayer>(PlayerController->Player))
		{
			UpdateHealthBar(TPSPlayer->GetHealth(), TPSPlayer->GetMaxHealth());
			TPSPlayer->OnHealthChanged.AddDynamic(this, &UTPSHealthBar::UpdateHealthBar);
		}
	}

	return bResult;
}

void UTPSHealthBar::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (APlayerController* PlayerController = TPSHelper::GetPlayerController())
	{
		if (ATPSPlayer* TPSPlayer = CastChecked<ATPSPlayer>(PlayerController->Player))
		{
			UpdateHealthBar(TPSPlayer->GetHealth(), TPSPlayer->GetMaxHealth());
			TPSPlayer->OnHealthChanged.RemoveDynamic(this, &UTPSHealthBar::UpdateHealthBar);
		}
	}
}

void UTPSHealthBar::UpdateHealthBar(float NewHealth, float MaxHealth)
{
	float Percent = NewHealth / MaxHealth;
	HealthBar->SetPercent(Percent);
}

