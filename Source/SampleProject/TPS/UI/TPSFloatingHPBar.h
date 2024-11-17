// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSFloatingHPBar.generated.h"

class ATPSCharacter;
class UProgressBar;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSFloatingHPBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void HealthChange(float NewHealth, float MaxHeath);
	void BindCharacter(ATPSCharacter* InCharacter);
	void UnbindCharacter();
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> HPBar;
	TWeakObjectPtr<ATPSCharacter> OwnerCharacter;
};
