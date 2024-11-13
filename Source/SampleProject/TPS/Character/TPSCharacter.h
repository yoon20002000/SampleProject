// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"
class UTPSCameraComponent;
class UTPSSpringArmComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class SAMPLEPROJECT_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> ShotAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTPSSpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTPSCameraComponent> CameraComp;
	
	UPROPERTY(EditAnywhere, Category="Ability", meta=(AllowPrivateAccess=true))
	int MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category="Ability", meta=(AllowPrivateAccess=true))
	int Health;
public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Shot(const FInputActionValue& InputActionValue);
};
