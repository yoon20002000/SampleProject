// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TPSCommonActivatableWidget.h"
#include "TPSHUDLayout.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSHUDLayout : public UTPSCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UTPSHUDLayout(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
protected:
	void HandleEscapeAction() const;
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};
