// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TPSAttributeComponent.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

class UTPSAttributeComponent;
class UTPSFloatingHPBar;
class UWidgetComponent;
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
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	
	float GetHealth()const;
	float GetMaxHealth()const;
	void AddHP(const int InValue);
	UTPSAttributeComponent* GetAttributeComp();
protected:
	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, UTPSAttributeComponent* OwningComp, float NewHealth, float Delta);
public:
	UPROPERTY(VisibleAnywhere, Category = UI)
	TObjectPtr<UWidgetComponent> HPBarWidget;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UTPSAttributeComponent> AttributeComp;
	
	UPROPERTY(EditDefaultsOnly, Category="UI", meta=(AllowPrivateAccess=true))
	int HPBarWidgetHeight = 180;
	UPROPERTY(EditDefaultsOnly, Category="UI", meta=(AllowPrivateAccess=true))
	FVector2D HPBarWidgetSize = FVector2D(150,50);
};
