// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "TPSInteractionInterface.generated.h"

struct FItem;

UINTERFACE(MinimalAPI)
class UTPSInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLEPROJECT_API ITPSInteractionInterface
{
	GENERATED_BODY()
public:
	virtual FItem* GetItemData() = 0;
	virtual void LookAtInteractionActor() = 0;
	virtual void Interaction() = 0;
};
