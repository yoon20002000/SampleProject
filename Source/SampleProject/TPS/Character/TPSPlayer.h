// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TPSCharacter.h"
#include "TPSPlayer.generated.h"

class UTPSAbilitySet;
struct FGameplayTag;
class UTPSInputConfig;
class UTPSActionComponent;
class UTPSInteractionComponent;
class UInputMappingContext;
class UInputAction;


UCLASS(Blueprintable)
class SAMPLEPROJECT_API ATPSPlayer : public ATPSCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void UninitAndDestroy() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

private:
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Interaction(const FInputActionValue& InputActionValue);
	void JumpAbilities();
	void StopJumpAbilities();
	void InitHUD();
	void UninitHUD();
	void PrintTags();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSSpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSCameraComponent> CameraComp;

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
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category="EI")
	TObjectPtr<UTPSInputConfig> InputConfig;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess), Category = "Components")
	TObjectPtr<UTPSInteractionComponent> InteractionComp;
};
