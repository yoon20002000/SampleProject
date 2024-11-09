// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "ModularGameModeBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MODULARGAMEPLAYACTOR_API AModularGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AModularGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};


UCLASS(Blueprintable)
class MODULARGAMEPLAYACTOR_API AModularGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AModularGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
