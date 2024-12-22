// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TPSHealthComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TPSCharacter.generated.h"

class UTPSGameplayAbility;
class UTPSHealthComponent;
class UTPSFloatingHPBar;
class UWidgetComponent;
class UInputMappingContext;
class UInputAction;
class UTPSCameraComponent;
class UTPSSpringArmComponent;
class UTPSActionComponent;
class UTPSAbilitySystemComponent;
class UTPSAbilitySet;
UCLASS()
class SAMPLEPROJECT_API ATPSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ATPSCharacter();
	virtual void BeginPlay() override;

	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	
	float GetHealth()const;
	float GetMaxHealth()const;
	
	UTPSHealthComponent* GetAttributeComp();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UTPSAbilitySystemComponent* GetTPSAbilitySystemComponent()const;
protected:
	void OnAbilitySystemInitialized();
	UFUNCTION()
	void OnHealthChanged(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* InstigatorActor);

public:
	UPROPERTY(VisibleAnywhere, Category = UI)
	TObjectPtr<UWidgetComponent> HPBarWidget;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UTPSHealthComponent> HealthComp;
	
	UPROPERTY(EditDefaultsOnly, Category="UI", meta=(AllowPrivateAccess=true))
	int HPBarWidgetHeight = 180;
	UPROPERTY(EditDefaultsOnly, Category="UI", meta=(AllowPrivateAccess=true))
	FVector2D HPBarWidgetSize = FVector2D(150,50);

	UPROPERTY(EditAnywhere, Category="EI")
	TArray<TObjectPtr<UTPSAbilitySet>> AbilitySets;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UTPSAbilitySystemComponent> AbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(allowPrivateAccess=true), Category = "Abilities")
	TArray<TSubclassOf<UTPSGameplayAbility>> CharacterAbilities;
};
