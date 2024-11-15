// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"

#include "Game/TPSCommonLocalPlayer.h"


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
