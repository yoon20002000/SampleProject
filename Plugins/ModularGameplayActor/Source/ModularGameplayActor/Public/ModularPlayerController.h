// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.generated.h"

/**
 * 
 */
class UObject;

UCLASS(Blueprintable)
class MODULARGAMEPLAYACTOR_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
};
