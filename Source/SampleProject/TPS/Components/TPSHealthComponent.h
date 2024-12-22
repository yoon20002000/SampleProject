#pragma once

#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "TPSHealthComponent.generated.h"

class UTPSHealthSet;
class UTPSHealthComponent;
class UTPSAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTPSHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FTPSHealth_AttributeChanged, UTPSHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SAMPLEPROJECT_API UTPSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTPSHealthComponent();
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "TPS|Health AttributeSet")
	void Initialize(UTPSAbilitySystemComponent* InASComp);

	bool IsAlive() const;
protected:
	void ClearGameplayTags();

	void HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* GameplayEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	void HandleMaxHealthChanged(AActor* Instigator, AActor* DamageCauser, const FGameplayEffectSpec* GameplayEffectSpec, float DamageMagnitude, float OldValue, float NewValue);
	void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* GameplayEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

public:
	UPROPERTY()
	FTPSHealth_AttributeChanged OnHealthChanged;
	UPROPERTY()
	FTPSHealth_AttributeChanged OnMaxHealthChanged;
	UPROPERTY()
	FTPSHealth_DeathEvent OnDeathStart;
	UPROPERTY()
	FTPSHealth_DeathEvent OnDeathEnd;
protected:
	UPROPERTY()
	TObjectPtr<UTPSAbilitySystemComponent> AbilitySystemComp;
	UPROPERTY()
	TObjectPtr<const UTPSHealthSet> HealthAttributeSet;
	
};
