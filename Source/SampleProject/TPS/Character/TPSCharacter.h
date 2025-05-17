// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TPSHealthComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TPSPlayerState.h"
#include "Components/TPSCharacterStatus.h"
#include "TPSCharacter.generated.h"

class UTPSCharacterStatus;
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
class UTPSNameplateSource;

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
	bool IsAlive() const;
	float GetHealth()const;
	float GetMaxHealth()const;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UTPSAbilitySystemComponent* GetTPSAbilitySystemComponent()const;

	bool GetCooldownRemainingForTag(const FGameplayTagContainer& CooldownTags, float& RemainingTime, float& CooldownDuration) const;
	
	void SetCharacterState(ECharacterState CharacterState);
	ECharacterState GetCharacterState()const;

	UTPSHealthComponent* GetHealthComponentOrNullptr() const;
	virtual void UninitAndDestroy();
	
// StatusSection
	FORCEINLINE UTPSCharacterStatus* GetCharacterStatus() const
	{
		return CharacterStatus;
	}
	FORCEINLINE int32 GetCharacterLevel() const
	{
		return GetCharacterStatus()->GetLevel();
	}
	FORCEINLINE void SetCharacterLevel(int32 InLevel)
	{
		CharacterStatus->SetLevel(InLevel);
	}
protected:
	void OnAbilitySystemInitialized();
	UFUNCTION()
	virtual void OnHealthChanged(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* InstigatorActor);

	virtual void InitializeAttributes();
	virtual void PossessedBy(AController* NewController) override;
	void SetPlayerInput(bool SetActive);
public:
	FOnAttackEndDelegate OnEndDelegate;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UTPSHealthComponent> HealthComp;
	
	UPROPERTY(EditAnywhere, Category="EI")
	TArray<TObjectPtr<UTPSAbilitySet>> AbilitySets;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UTPSAbilitySystemComponent> AbilitySystemComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(allowPrivateAccess=true), Category = "Abilities")
	TArray<TSubclassOf<UTPSGameplayAbility>> CharacterAbilities;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadonly, meta=(AllowPrivateAccess))
	ECharacterState CurrentState;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UTPSNameplateSource> NameplateSource;
	
	bool bIsPlayerControlled;
	bool bCanBeDamaged;
// Status Section
public:
	UPROPERTY(EditAnywhere, Category="Character Status")
	TObjectPtr<UTPSCharacterStatus> CharacterStatus;
private:
	UPROPERTY()
	TObjectPtr<UTPSCombatAttributeSet> CombatSet;
};
