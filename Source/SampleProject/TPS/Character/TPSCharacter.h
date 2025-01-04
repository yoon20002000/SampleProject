// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TPSHealthComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TPSPlayerState.h"
#include "TPSCharacter.generated.h"

class UTPSCombatAttributeSet;
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

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate)

UCLASS()
class SAMPLEPROJECT_API ATPSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ATPSCharacter();
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDeathStart(AActor* OwningActor);
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	
	float GetHealth()const;
	float GetMaxHealth()const;
	
	UTPSHealthComponent* GetHealthAttributeComp();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UTPSAbilitySystemComponent* GetTPSAbilitySystemComponent()const;

	void SetCharacterState(ECharacterState CharacterState);
	ECharacterState GetCharacterState()const;

	void UninitAndDestroy();
protected:
	void OnAbilitySystemInitialized();
	UFUNCTION()
	void OnHealthChanged(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* InstigatorActor);

	virtual void InitializeAttributes();
	virtual void PossessedBy(AController* NewController) override;
	void SetPlayerInput(bool SetActive);
public:
	UPROPERTY(VisibleAnywhere, Category = UI)
	TObjectPtr<UWidgetComponent> HPBarWidget;

	FOnAttackEndDelegate OnAttackEnd;
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

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadonly, meta=(AllowPrivateAccess))
	ECharacterState CurrentState;

	bool bIsPlayerControlled;
	bool bCanBeDamaged;
private:
	UPROPERTY()
	TObjectPtr<UTPSCombatAttributeSet> CombatSet;
};
