// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameUIPolicy.generated.h"

class UYCommonLocalPlayer;
/**
 * 
 */
UCLASS()
class COMMONGAME_API UYGameUIPolicy : public UObject
{
	GENERATED_BODY()

private:
	void NotifyPlayerAdded(UYCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UYCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UYCommonLocalPlayer* LocalPlayer);

	friend class UYGameUIManagerSubsystem;
};
