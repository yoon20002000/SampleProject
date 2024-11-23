// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/InteractionActor.h"
#include "HPPotion.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API AHPPotion : public AInteractionActor
{
	GENERATED_BODY()

public:
	AHPPotion();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HP Potion")
	int32 RecoverPoint;
};
