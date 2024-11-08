// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

#include "CommonLocalPlayer.h"
#include "GameUIManagerSubsystem.h"
#include "LogCommonGame.h"

int32 UYTPSGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int ReturnVal =  Super::AddLocalPlayer(NewPlayer, UserId);

	if(ReturnVal != INDEX_NONE)
	{
		if(PrimaryPlayer.IsValid() == false)
		{
			//UE_LOG(LogCommonGame, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"),*NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}
		GetSubsystem<UYGameUIManagerSubsystem>()->NotifyPlayerAdded(CastChecked<UYCommonLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}
