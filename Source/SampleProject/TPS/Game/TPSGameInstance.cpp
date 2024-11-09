// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

#include "CommonGame/CommonLocalPlayer.h"
#include "CommonGame/GameUIManagerSubsystem.h"

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
		GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(CastChecked<UCommonLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}
