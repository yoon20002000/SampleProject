// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ATPSPlayerState();
	virtual void BeginPlay() override;

	void AddDeathCount();
	void ClearDeathCount();
protected:
	UPROPERTY(Transient)
	uint32 DeathCount;
};
