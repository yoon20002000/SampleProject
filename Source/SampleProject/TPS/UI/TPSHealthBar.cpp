// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSHealthBar.h"

#include "Character/TPSPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Game/AbilitySystem/TPSHealthSet.h"

void UTPSHealthBar::InitHealthBar(const ATPSPlayer* TPSPlayer) 
{
	if (TPSPlayer == nullptr)
	{
		return;
	}
	
	CurrentHealth = TPSPlayer->GetHealth();
	CurrentMaxHealth = TPSPlayer->GetMaxHealth(); 
	UpdateUI();
	
	if (UTPSAbilitySystemComponent* ASC = TPSPlayer->GetTPSAbilitySystemComponent())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UTPSHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UTPSHealthSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthChanged);
	}
}

void UTPSHealthBar::UnInitHealthBar(const ATPSPlayer* TPSPlayer)
{
	if (TPSPlayer == nullptr)
	{
		return;
	}
	
	if (UTPSAbilitySystemComponent* ASC = TPSPlayer->GetTPSAbilitySystemComponent())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UTPSHealthSet::GetHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UTPSHealthSet::GetMaxHealthAttribute()).RemoveAll(this);
	}
}

bool UTPSHealthBar::Initialize()
{
	bool bResult = Super::Initialize();

	return bResult;
}

void UTPSHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTPSHealthBar::BeginDestroy()
{
	Super::BeginDestroy();

	if (ATPSPlayer* TPSPlayer = GetTPSPlayer())
	{
		UnInitHealthBar(TPSPlayer);
	}
}

void UTPSHealthBar::UpdateUI()
{
	HealthPointText->SetText(FText::AsNumber(CurrentHealth));
	const float Percent = CurrentHealth / CurrentMaxHealth;
	HealthBar->SetPercent(Percent);
}

void UTPSHealthBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentHealth = Data.NewValue;
	UpdateUI();
}

void UTPSHealthBar::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentMaxHealth = Data.NewValue;
	UpdateUI();
}
