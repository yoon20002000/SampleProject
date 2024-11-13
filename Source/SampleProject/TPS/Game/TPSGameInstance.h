// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

class UGameDataAsset;
class UTPSUIManager;
class ATPSGameMode;

/**
 * 
 */
UCLASS(Blueprintable)
class SAMPLEPROJECT_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;
	virtual void ReturnToMainMenu() override;
	virtual void Shutdown() override;

private:
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;

};
