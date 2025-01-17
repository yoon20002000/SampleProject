// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"

#include "TPSCharacter.h"
#include "TPSPlayer.h"
#include "TPSPlayerState.h"
#include "TPSSystemManager.h"
#include "Components/TPSIndicatorManagerComponent.h"
#include "Game/TPSCommonLocalPlayer.h"
#include "Game/TPSGameStateManager.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"


ATPSPlayerController::ATPSPlayerController()
{
	IndicatorManagerComp = CreateDefaultSubobject<UTPSIndicatorManagerComponent>(TEXT("Indicator Manager Comp"));
}

void ATPSPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	
	if (UTPSCommonLocalPlayer* LocalPlayer = Cast<UTPSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);

		if (PlayerState)
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
		}
	}
}

void ATPSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (UTPSCommonLocalPlayer* LocalPlayer = Cast<UTPSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, InPawn);
	}
}

void ATPSPlayerController::OnPossess(APawn* APawn)
{
	Super::OnPossess(APawn);
	
	if (UTPSCommonLocalPlayer* LocalPlayer = Cast<UTPSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, APawn);
	}
}

void ATPSPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (UTPSCommonLocalPlayer* LocalPlayer = Cast<UTPSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, nullptr);
	}
}

void ATPSPlayerController::SetGamePause(const bool bIsGamePause)
{
	SetShowMouseCursor(bIsGamePause);

	if (bIsGamePause == false)
	{
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
	}
	
	SetPause(bIsGamePause);
}

void ATPSPlayerController::SetGameEnd()
{
	DisableInput(this);
	UTPSSystemManager::Get()->SetGameState(EGameplayState::GameResult);
}

void ATPSPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UTPSAbilitySystemComponent* TPSASComp = GetTPSAbilitySystemComponent())
	{
		TPSASComp->ProcessAbilityInput(DeltaTime,bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

const ATPSPlayerState* ATPSPlayerController::GetTPSPlayerState() const
{
	return CastChecked<ATPSPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UTPSAbilitySystemComponent* ATPSPlayerController::GetTPSAbilitySystemComponent() const
{
	ATPSPlayer* TPSPlayer = Cast<ATPSPlayer>(GetPawn());
	if (TPSPlayer == nullptr)
	{
		return nullptr;	
	}

	return Cast<UTPSAbilitySystemComponent>(TPSPlayer->GetAbilitySystemComponent());
}

void ATPSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (PlayerState)
	{
		if (UTPSCommonLocalPlayer* LocalPlayer = Cast<UTPSCommonLocalPlayer>(Player))
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
		}
	}
}
