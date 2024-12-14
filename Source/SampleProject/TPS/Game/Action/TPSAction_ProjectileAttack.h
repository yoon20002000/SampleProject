// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Action/TPSAction.h"
#include "TPSAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSAction_ProjectileAttack : public UTPSAction
{
	GENERATED_BODY()

public:
	UTPSAction_ProjectileAttack();
	virtual void StartAction_Implementation(AActor* Instigator) override;


};
