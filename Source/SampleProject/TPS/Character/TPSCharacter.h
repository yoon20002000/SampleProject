// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UTPSCameraComponent;
class UTPSSpringArmComponent;


UCLASS()
class SAMPLEPROJECT_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ATPSCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	float GetHealth()const;
	float GetMaxHealth()const;
protected:
	virtual void ChangeHealth(float InNewHealth);

protected:
	UPROPERTY(EditAnywhere, Category="Ability", meta=(AllowPrivateAccess=true))
	int MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category="Ability", meta=(AllowPrivateAccess=true))
	int Health;
	
};
