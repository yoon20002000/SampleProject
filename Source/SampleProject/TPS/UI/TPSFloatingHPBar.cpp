// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSFloatingHPBar.h"

#include "Character/TPSCharacter.h"
#include "Components/ProgressBar.h"

void UTPSFloatingHPBar::UpdateFloatingHPBar(const float Health, const float MaxHealth)
{
	HPBar->SetPercent(Health / MaxHealth);
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
		if (UTPSHealthComponent* AttributeComp = OwnerCharacter.Get()->GetHealthComponentOrNullptr())
		{
			UpdateFloatingHPBar(AttributeComp->GetHealth(), AttributeComp->GetMaxHealth());
			AttributeComp->OnHealthChanged.AddDynamic(this, &ThisClass::UpdateUIs);	
		}
	}
}

void UTPSFloatingHPBar::UnbindCharacter()
{
	if (OwnerCharacter.IsValid() == true)
	{
		if (UTPSHealthComponent* HealthComp = OwnerCharacter.Get()->GetHealthComponentOrNullptr())
		{
			HealthComp->OnHealthChanged.RemoveDynamic(this, &UTPSFloatingHPBar::UpdateUIs);
		}
	}
	OwnerCharacter = nullptr;
}

void UTPSFloatingHPBar::UpdateUIs(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue,
	AActor* Instigator)
{
	if (HPBar == nullptr)
	{
		return;
	}
	
	UpdateFloatingHPBar(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
}
