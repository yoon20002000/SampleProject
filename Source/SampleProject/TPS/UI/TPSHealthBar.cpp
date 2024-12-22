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
		TPSPlayer->GetAttributeComp()->HandleHealthChanged.AddDynamic(this, &UTPSHealthBar::UpdateUIs);
	}
}

void UTPSHealthBar::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (ATPSPlayer* TPSPlayer = GetTPSPlayer())
	{
		TPSPlayer->GetAttributeComp()->HandleHealthChanged.RemoveDynamic(this, &UTPSHealthBar::UpdateUIs);
	}
}

void UTPSHealthBar::UpdateUIs(AActor* InstigatorActor, UTPSHealthComponent* AttributeComp, float NewHealth,
	float Delta)
{
	if (AttributeComp == nullptr)
	{
		return;
	}
	
	UpdateHealthBar(AttributeComp->GetHealth(), AttributeComp->GetMaxHealth());
	UpdateHealthPoint(AttributeComp->GetHealth());
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
