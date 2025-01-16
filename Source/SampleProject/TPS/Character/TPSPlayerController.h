// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSPlayerState.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/TPSNameplateManager.h"
#include "TPSPlayerController.generated.h"

class UTPSIndicatorManagerComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class SAMPLEPROJECT_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPSPlayerController();
	virtual void ReceivedPlayer() override;	
	virtual void SetPawn(APawn* InPawn) override;
	virtual void OnPossess(class APawn* APawn) override;
	virtual void OnUnPossess() override;
	virtual void SetGamePause(const bool bIsGamePause);
	virtual void SetGameEnd();
	
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	const ATPSPlayerState* GetTPSPlayerState() const;
	UTPSAbilitySystemComponent* GetTPSAbilitySystemComponent() const;
	
protected:
	virtual void OnRep_PlayerState() override;
	
private:
	FInputModeGameOnly InputGameOnly;
	FInputModeUIOnly InputUIOnly;
	UPROPERTY()
	TObjectPtr<UTPSIndicatorManagerComponent> IndicatorManagerComp;
	UPROPERTY()
	TObjectPtr<UTPSNameplateManager> NamePlateManager;
};
