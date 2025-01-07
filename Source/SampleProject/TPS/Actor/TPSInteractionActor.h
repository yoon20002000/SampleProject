// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TPSInteractionActor.generated.h"

struct FActiveGameplayEffectHandle;
class UGameplayEffect;
class UTPSAbilitySystemComponent;
class USphereComponent;

UCLASS()
class SAMPLEPROJECT_API ATPSInteractionActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATPSInteractionActor();
	FActiveGameplayEffectHandle ApplyGE(UTPSAbilitySystemComponent* AbilitySystemComponent);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

private:
	UPROPERTY(EditDefaultsOnly, Category="Interaction", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> EffectClass;
};
