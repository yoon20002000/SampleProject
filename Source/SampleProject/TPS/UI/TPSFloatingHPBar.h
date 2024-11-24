// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TPSAttributeComponent.h"
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
	void UpdateFloatingHPBar(const float Health, const float MaxHealth);
	UFUNCTION()
	void UpdateUIs(AActor* InstigatorActor, UTPSAttributeComponent* AttributeComp, float NewHealth, float Delta);
	void BindCharacter(ATPSCharacter* InCharacter);
	void UnbindCharacter();

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> HPBar;
	TWeakObjectPtr<ATPSCharacter> OwnerCharacter;
};
