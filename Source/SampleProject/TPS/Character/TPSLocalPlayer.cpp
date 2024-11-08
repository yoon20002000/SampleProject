// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSLocalPlayer.h"

UYTPSLocalPlayer::UYTPSLocalPlayer()
{
}

bool UYTPSLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	OnPlayerControllerChanged(PlayerController);
	
	return bResult;
}

void UYTPSLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	
}
