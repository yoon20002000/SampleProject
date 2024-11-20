// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"

#include "TPSSystemManager.h"
#include "Game/TPSCommonLocalPlayer.h"
#include "Game/TPSGameStateManager.h"


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
	UTPSSystemManager::Get()->SetGameState(EGameplayState::GameResult);
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
