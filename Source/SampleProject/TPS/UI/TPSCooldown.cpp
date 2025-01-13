// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSCooldown.h"

#include "CommonTextBlock.h"
#include "Character/TPSCharacter.h"
#include "Components/Image.h"


UTPSCooldown::UTPSCooldown()
{
	FormattingOptions.SetMaximumFractionalDigits(2);
}

void UTPSCooldown::InitCooldownUI(ATPSCharacter* InTargetCharacter)
{
	if (InTargetCharacter == nullptr)
	{
		return;
	}

	TargetCharacter = InTargetCharacter;
}

void UTPSCooldown::UninitCooldownUI(ATPSCharacter* InTargetCharacter)
{
	if (InTargetCharacter == nullptr)
	{
		return;
	}
	TargetCharacter = nullptr;
}
void UTPSCooldown::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UTPSAbilitySystemComponent* ASC = TargetCharacter->GetTPSAbilitySystemComponent())
	{
		float RemaingTime;
		float CooldownDuration;
		
		FGameplayTagContainer TargetTagContainer;
		TargetTagContainer.AddTag(TargetTag);
		TargetCharacter->GetCooldownRemainingForTag(TargetTagContainer,RemaingTime,CooldownDuration);
		UpdateUI(RemaingTime);
	}
}
void UTPSCooldown::UpdateUI(float RemainTime)
{
	if (RemainTime > 0)
	{
		if (RemainTimeText->GetVisibility() != ESlateVisibility::Visible)
		{
			RemainTimeText->SetVisibility(ESlateVisibility::Visible);
		}
		
		FText Text = FText::AsNumber(RemainTime, &FormattingOptions);
		RemainTimeText->SetText(Text);
		SkillIcon->SetBrushTintColor(CooldownColor);
	}
	else
	{
		RemainTimeText->SetVisibility(ESlateVisibility::Hidden);
		SkillIcon->SetBrushTintColor(NormalColor);
	}
}
