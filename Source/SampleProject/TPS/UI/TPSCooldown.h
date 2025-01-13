// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "UI/TPSCommonUserWidget.h"
#include "TPSCooldown.generated.h"

class ATPSCharacter;
class UTPSAbilitySystemComponent;
class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSCooldown : public UTPSCommonUserWidget
{
	GENERATED_BODY()

public:
	UTPSCooldown();

	void InitCooldownUI(ATPSCharacter* InTargetCharacter);
	void UninitCooldownUI(ATPSCharacter* InTargetCharacter);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	void UpdateUI(float RemainTime);
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSlateColor CooldownColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSlateColor NormalColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag TargetTag;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SkillIcon;
private:
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess= true))
	TObjectPtr<UCommonTextBlock> RemainTimeText;

	TWeakObjectPtr<ATPSCharacter> TargetCharacter;
	FNumberFormattingOptions FormattingOptions;
};
