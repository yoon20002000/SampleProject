// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

class UTPSFloatingHPBar;
class UWidgetComponent;
class UInputMappingContext;
class UInputAction;
class UTPSCameraComponent;
class UTPSSpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHeath);

UCLASS()
class SAMPLEPROJECT_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ATPSCharacter();
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	
	float GetHealth()const;
	float GetMaxHealth()const;
	void AddHP(const int InValue); 
protected:
	virtual void ChangeHealth(float InNewHealth);

public:
	UPROPERTY(VisibleAnywhere, Category = UI)
	TObjectPtr<UWidgetComponent> HPBarWidget;

	FOnHealthChanged OnHealthChanged;
protected:
	UPROPERTY(EditAnywhere, Category="Ability", meta=(AllowPrivateAccess=true))
	int MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category="Ability", meta=(AllowPrivateAccess=true))
	int Health;
	UPROPERTY(EditDefaultsOnly, Category="UI", meta=(AllowPrivateAccess=true))
	int HPBarWidgetHeight = 180;
	UPROPERTY(EditDefaultsOnly, Category="UI", meta=(AllowPrivateAccess=true))
	FVector2D HPBarWidgetSize = FVector2D(150,50);
};
