// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSNameplate.h"

#include "CommonTextBlock.h"
#include "TPSIndicatorDescriptor.h"
#include "Character/TPSCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSIndicatorManagerComponent.h"
#include "Game/AbilitySystem/TPSAttributeSet.h"
#include "Game/AbilitySystem/TPSHealthSet.h"
#include "Game/AbilitySystem/TPSHealthSet.h"


UTPSNameplate::UTPSNameplate()
{
}

void UTPSNameplate::BindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	check(IndicatorDescriptor != nullptr);

	TargetIndicator = IndicatorDescriptor;
	IndicatorDescriptor->SetDesiredVisibility(false);
	InitNameplate(IndicatorDescriptor);
}

void UTPSNameplate::UnbindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	check(IndicatorDescriptor != nullptr);
	IndicatorDescriptor->SetDesiredVisibility(false);

	if (ATPSCharacter* Character = Cast<ATPSCharacter>(IndicatorDescriptor->GetDataObject()))
	{
		if (UTPSAbilitySystemComponent* ASC = Character->GetTPSAbilitySystemComponent())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UTPSHealthSet::GetHealthAttribute()).RemoveAll(this);
			ASC->GetGameplayAttributeValueChangeDelegate(UTPSHealthSet::GetMaxHealthAttribute()).RemoveAll(this);
		}
	}
	TargetIndicator = nullptr;
}

void UTPSNameplate::InitNameplate(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	check(IndicatorDescriptor != nullptr);
	IndicatorDescriptor->SetDesiredVisibility(true);
	SetVisibility(ESlateVisibility::Visible);

	if (APawn* IndicatorPawn = Cast<APawn>(IndicatorDescriptor->GetDataObject()))
	{
		SetNameText(IndicatorPawn->GetName());

		if (ATPSCharacter* Character = Cast<ATPSCharacter>(IndicatorDescriptor->GetDataObject()))
		{
			if (UTPSAbilitySystemComponent* ASC = Character->GetTPSAbilitySystemComponent())
			{
				CurrentHealth = Character->GetHealth();
				CurrentMaxHealth = Character->GetMaxHealth();

				ASC->GetGameplayAttributeValueChangeDelegate(UTPSHealthSet::GetHealthAttribute()).AddUObject(
					this, &ThisClass::OnHealthChanged);
				ASC->GetGameplayAttributeValueChangeDelegate(UTPSHealthSet::GetMaxHealthAttribute()).AddUObject(
					this, &ThisClass::OnMaxHealthChanged);

				SetHealthBarPercent();
			}
		}
	}
}

void UTPSNameplate::UninitNameplate()
{
	if (ATPSCharacter* IndicatorCharacter = Cast<ATPSCharacter>(TargetIndicator->GetDataObject()))
	{
		if (UTPSIndicatorManagerComponent* IndicatorManager = IndicatorCharacter->GetController()->GetComponentByClass<UTPSIndicatorManagerComponent>())
		{
			IndicatorManager->RemoveIndicator(TargetIndicator.Get());
		}
	}
	UnbindIndicator(TargetIndicator.Get());
}

void UTPSNameplate::SetNameText(const FString& InNameText)
{
	CharacterNameText->SetText(FText::FromString(InNameText));
}


void UTPSNameplate::SetHealthBarPercent()
{
	if (CurrentMaxHealth == 0)
	{
		return;
	}
	HPBar->SetPercent(CurrentHealth / CurrentMaxHealth);
}

void UTPSNameplate::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentHealth = Data.NewValue;
	if (CurrentHealth > 0)
	{
		SetHealthBarPercent();
	}
	else
	{
		UninitNameplate();
	}
}

void UTPSNameplate::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	CurrentMaxHealth = Data.NewValue;
	SetHealthBarPercent();
}
