// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

#include "CommonGame/CommonLocalPlayer.h"
#include "CommonGame/TPSGameUIManagerSubsystem.h"

int32 UTPSGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int ReturnVal =  Super::AddLocalPlayer(NewPlayer, UserId);

	if(ReturnVal != INDEX_NONE)
	{
		if(PrimaryPlayer.IsValid() == false)
		{
			//UE_LOG(LogCommonGame, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"),*NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}
		GetSubsystem<UTPSGameUIManagerSubsystem>()->NotifyPlayerAdded(CastChecked<UCommonLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}
