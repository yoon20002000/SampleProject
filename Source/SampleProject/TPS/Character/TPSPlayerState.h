// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPSPlayerState.generated.h"

class ATPSPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnKillCountChanged,
													ATPSPlayerState*, PlayerState,
													uint32, KillCount,
													uint8, Delta);
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ATPSPlayerState();
	virtual void BeginPlay() override;

	void AddKillCount();
	void ClearKillCount();
	void UpdateKillCount(uint32 InKillCount);
public:
	FOnKillCountChanged OnKillCountChanged;
protected:
	uint32 KillCount;
};
