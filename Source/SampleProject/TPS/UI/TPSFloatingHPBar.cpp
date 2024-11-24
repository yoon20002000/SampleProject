// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSFloatingHPBar.h"

#include "Character/TPSCharacter.h"
#include "Components/ProgressBar.h"

void UTPSFloatingHPBar::UpdateFloatingHPBar(const float Health, const float MaxHealth)
{
	HPBar->SetPercent(Health / MaxHealth);
}

void UTPSFloatingHPBar::UpdateUIs(AActor* InstigatorActor, UTPSAttributeComponent* AttributeComp, float NewHealth,
                                        float Delta)
{
	if (HPBar == nullptr)
	{
		return;
	}
	
	UpdateFloatingHPBar(AttributeComp->GetHealth(), AttributeComp->GetMaxHealth());
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
		UTPSAttributeComponent* AttributeComp = OwnerCharacter.Get()->GetAttributeComp();
		UpdateFloatingHPBar(AttributeComp->GetHealth(), AttributeComp->GetMaxHealth());
		AttributeComp->OnHealthChanged.AddDynamic(this, &UTPSFloatingHPBar::UpdateUIs);
	}
}

void UTPSFloatingHPBar::UnbindCharacter()
{
	if (OwnerCharacter.IsValid() == true)
	{
		OwnerCharacter.Get()->GetAttributeComp()->OnHealthChanged.RemoveDynamic(this, &UTPSFloatingHPBar::UpdateUIs);
	}
	OwnerCharacter = nullptr;
}
