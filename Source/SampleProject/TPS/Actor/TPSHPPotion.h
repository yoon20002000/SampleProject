// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TPSInteractionActor.h"
#include "TPSHPPotion.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSHPPotion : public ATPSInteractionActor
{
	GENERATED_BODY()

public:
	ATPSHPPotion();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HP Potion")
	int32 RecoverPoint;
};
