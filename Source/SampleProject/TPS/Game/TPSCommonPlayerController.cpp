// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSCommonPlayerController.h"

#include "TPSCommonLocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TPSCommonPlayerController)

class APawn;

ATPSCommonPlayerController::ATPSCommonPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ATPSCommonPlayerController::ReceivedPlayer()
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

void ATPSCommonPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (UTPSCommonLocalPlayer* LocalPlayer = Cast<UTPSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, InPawn);
	}
}

void ATPSCommonPlayerController::OnPossess(APawn* APawn)
{
	Super::OnPossess(APawn);
	
	if (UTPSCommonLocalPlayer* LocalPlayer = Cast<UTPSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, APawn);
	}
}

void ATPSCommonPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	if (UTPSCommonLocalPlayer* LocalPlayer = Cast<UTPSCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, nullptr);
	}
}

void ATPSCommonPlayerController::OnRep_PlayerState()
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
