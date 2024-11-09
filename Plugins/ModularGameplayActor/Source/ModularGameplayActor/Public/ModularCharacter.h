// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ModularCharacter.generated.h"

UCLASS(Blueprintable)
class MODULARGAMEPLAYACTOR_API AModularCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
