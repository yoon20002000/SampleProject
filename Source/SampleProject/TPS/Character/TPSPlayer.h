// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TPSCharacter.h"
#include "TPSPlayer.generated.h"

class UTPSInventoryComponent;
class UTPSInteractionIndicatorComponent;
class UTPSAbilitySet;
struct FGameplayTag;
class UTPSInputConfig;
class UTPSActionComponent;
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

	UTPSInventoryComponent* GetInventoryComponent() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

private:
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void InitHUD();
	void UninitHUD();
	void PrintTags();
	bool CanMove() const;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSSpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTPSCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="EI")
	TObjectPtr<UTPSInputConfig> InputConfig;
private:
	UPROPERTY(Transient, EditInstanceOnly, Category = "Debug", meta = (AllowPrivateAccess = true))
	bool bPrintTag;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer MoveLimitTagContainer;
	UPROPERTY(Transient,EditDefaultsOnly)
	TObjectPtr<UTPSInteractionIndicatorComponent> InteractionIndicatorComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTPSInventoryComponent> InventoryComp;
};
