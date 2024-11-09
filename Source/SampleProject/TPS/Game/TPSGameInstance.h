// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UYTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
private:
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
	
};
