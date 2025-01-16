// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSIndicatorWidgetInterface.h"
#include "UI/TPSCommonUserWidget.h"
#include "TPSNameplate.generated.h"

class UCommonTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSNameplate : public UTPSCommonUserWidget, public ITPSIndicatorWidgetInterface
{
	GENERATED_BODY()
public:
	UTPSNameplate();

	void BindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor);
	void UnbindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor);
private:
	UPROPERTY(meta=(AllowPrivateAccess=true, BindWidget))
	TObjectPtr<UProgressBar> HPBar;
	UPROPERTY(meta=(AllowPrivateAccess=true, BindWidget))
	TObjectPtr<UCommonTextBlock> CharacterNameText;
};
