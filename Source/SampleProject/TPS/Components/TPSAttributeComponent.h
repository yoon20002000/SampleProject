// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged,
                                              AActor*,
                                              InstigatorActor,
                                              UTPSAttributeComponent*,
                                              AttributeComp,
                                              float,
                                              NewHealth,
                                              float,
                                              Delta
                                              );

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SAMPLEPROJECT_API UTPSAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTPSAttributeComponent();
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	static UTPSAttributeComponent* GetAttributes(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* InActor);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float ActuableDelta);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastManaChanged(AActor* InstigatorActor, float NewMana, float ActuableDelta);
	
	bool IsAlive() const;
	float GetHealth() const;
	float GetMaxHealth() const;

	float GetMana() const;
	float GetMaxMana() const;

public:
	bool ApplyHealthChange(AActor* InstigatorActor, const float Delta);
	bool ApplyManaChange(AActor* InstigatorActor, const float Delta);

public:
	UPROPERTY(Transient)
	FOnAttributeChanged OnHealthChanged;
	UPROPERTY(Transient)
	FOnAttributeChanged OnManaChanged;

protected:
	UPROPERTY(EditAnywhere, Category="Attribute", meta=(AllowPrivateAccess=true))
	float MaxHealth;
	UPROPERTY(Transient)
	float Health;

	UPROPERTY(EditAnywhere, Category="Attribute", meta=(AllowPrivateAccess=true))
	float MaxMana;
	UPROPERTY(Transient)
	float Mana;
};
