// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSHealthBar.h"

#include "Character/TPSPlayer.h"
#include "Components/ProgressBar.h"
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
		UpdateHealthPoint(TPSPlayer->GetHealth());
		TPSPlayer->GetHealthAttributeComp()->OnHealthChanged.AddDynamic(this, &ThisClass::UpdateUIs);
	}
}

void UTPSHealthBar::UpdateUIs(UTPSHealthComponent* HealthComp, float OldValue, float NewValue, AActor* Actor)
{
	if (HealthComp == nullptr)
	{
		return;
	}
	
	UpdateHealthBar(HealthComp->GetHealth(), HealthComp->GetMaxHealth());
	UpdateHealthPoint(HealthComp->GetHealth());
}

void UTPSHealthBar::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (ATPSPlayer* TPSPlayer = GetTPSPlayer())
	{
		TPSPlayer->GetHealthAttributeComp()->OnHealthChanged.RemoveDynamic(this, &ThisClass::UpdateUIs);
	}
}

void UTPSHealthBar::UpdateHealthBar(const float NewHealth, const float MaxHealth)
{
	const float Percent = NewHealth / MaxHealth;
	HealthBar->SetPercent(Percent);
}

void UTPSHealthBar::UpdateHealthPoint(float NewHealth)
{
	HealthPointText->SetText(FText::AsNumber(NewHealth));
}
