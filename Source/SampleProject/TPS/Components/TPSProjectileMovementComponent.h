// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TPSProjectileMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
public:
	void Reset();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
