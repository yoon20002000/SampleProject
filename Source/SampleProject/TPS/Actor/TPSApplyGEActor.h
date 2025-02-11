// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSInteractionActorBase.h"
#include "GameFramework/Actor.h"
#include "TPSApplyGEActor.generated.h"

struct FActiveGameplayEffectHandle;
class UGameplayEffect;
class UTPSAbilitySystemComponent;
class USphereComponent;

UCLASS()
class SAMPLEPROJECT_API ATPSApplyGEActor : public ATPSInteractionActorBase
{
	GENERATED_BODY()

public:
	ATPSApplyGEActor();
	
	FString GetInteractionDescription() const;
	// ITPSInteractionWithActorInterface Start
	virtual void Interaction(AActor* InteractionInstigatorActor) override;
	// ITPSInteractionWithActorInterface End
private:
	FActiveGameplayEffectHandle ApplyGE(UTPSAbilitySystemComponent* AbilitySystemComponent);
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USphereComponent> SphereComp;
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

private:
	UPROPERTY(EditDefaultsOnly, Category="Interaction", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> EffectClass;
	UPROPERTY(EditDefaultsOnly, Category="Interaction", meta=(AllowPrivateAccess=true))
	FString InteractionDescription;
};
