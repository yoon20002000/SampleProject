// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSFloatingHPBar.h"

#include "Character/TPSCharacter.h"
#include "Components/ProgressBar.h"

void UTPSFloatingHPBar::HealthChange(float NewHealth, float MaxHeath)
{
	if (HPBar == nullptr)
	{
		return;
	}
	HPBar->SetPercent(NewHealth / MaxHeath);
}

void UTPSFloatingHPBar::BindCharacter(ATPSCharacter* InCharacter)
{
	if (InCharacter == nullptr)
	{
		return;
	}

	OwnerCharacter = InCharacter;
	if (OwnerCharacter.IsValid() == true)
	{
		OwnerCharacter.Get()->OnHealthChanged.AddDynamic(this, &UTPSFloatingHPBar::HealthChange);
	}
}

void UTPSFloatingHPBar::UnbindCharacter()
{
	if (OwnerCharacter.IsValid() == true)
	{
		OwnerCharacter.Get()->OnHealthChanged.RemoveDynamic(this, &UTPSFloatingHPBar::HealthChange);
	}
	OwnerCharacter = nullptr;
}
