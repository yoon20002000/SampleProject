// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "ModularGameStateBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MODULARGAMEPLAYACTOR_API AModularGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
