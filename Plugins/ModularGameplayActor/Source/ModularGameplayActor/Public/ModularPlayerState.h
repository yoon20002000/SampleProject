// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.generated.h"

/**
 * 
 */
namespace EEndPlayReason { enum Type : int;}

UCLASS(Blueprintable)
class MODULARGAMEPLAYACTOR_API AModularPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;
};
