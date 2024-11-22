// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSHealthBar.h"

#include "Character/TPSPlayer.h"
#include "Components/ProgressBar.h"
//#include "CommonNumericTextBlock.h"
#include "Components/TextBlock.h"

bool UTPSHealthBar::Initialize()
{
	bool bResult = Super::Initialize();
	
	return bResult;
}

void UTPSHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	if (ATPSPlayer* TPSPlayer = GetTPSPlayer())
	{
		UpdateHealthBar(TPSPlayer->GetHealth(), TPSPlayer->GetMaxHealth());
		TPSPlayer->OnHealthChanged.AddDynamic(this, &UTPSHealthBar::UpdateHealthBar);
	}
}

void UTPSHealthBar::BeginDestroy()
{
	Super::BeginDestroy();

	UE_LOG(LogTemp, Log, TEXT("Is going destroy!!!"));
	if (ATPSPlayer* TPSPlayer = GetTPSPlayer())
	{
		TPSPlayer->OnHealthChanged.RemoveDynamic(this, &UTPSHealthBar::UpdateHealthBar);
	}
}

void UTPSHealthBar::UpdateHealthBar(const float NewHealth, const float MaxHealth)
{
	const float Percent = NewHealth / MaxHealth;
	HealthBar->SetPercent(Percent);
	UpdateHealthPoint(NewHealth);
}

void UTPSHealthBar::UpdateHealthPoint(float NewHealth)
{
	HealthPointText->SetText(FText::AsNumber(NewHealth));
}
