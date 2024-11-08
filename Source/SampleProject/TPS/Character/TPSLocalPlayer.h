// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "TPSLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UYTPSLocalPlayer : public UYCommonLocalPlayer
{
	GENERATED_BODY()
public:
	UYTPSLocalPlayer();
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
protected:
	void OnPlayerControllerChanged(APlayerController* NewController);
};
