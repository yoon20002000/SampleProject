// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

#include "CommonUISettings.h"
#include "GameplayTagContainer.h"
#include "ICommonUIModule.h"
#include "CommonGame/LogTPSGame.h"
#include "CommonGame/TPSCommonLocalPlayer.h"
#include "CommonGame/TPSGameUIManagerSubsystem.h"

void UTPSGameInstance::Init()
{
	Super::Init();
	FGameplayTagContainer PlatformTraits = ICommonUIModule::GetSettings().GetPlatformTraits();
}

int32 UTPSGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int ReturnVal =  Super::AddLocalPlayer(NewPlayer, UserId);

	if(ReturnVal != INDEX_NONE)
	{
		if(PrimaryPlayer.IsValid() == false)
		{
			UE_LOG(LogTPSGame, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"),*NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}
		GetSubsystem<UTPSGameUIManagerSubsystem>()->NotifyPlayerAdded(CastChecked<UTPSCommonLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}

bool UTPSGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		//TODO: do we want to fall back to another player?
		PrimaryPlayer.Reset();
		UE_LOG(LogTPSGame, Log, TEXT("RemoveLocalPlayer: Unsetting Primary Player from %s"), *ExistingPlayer->GetName());
	}
	GetSubsystem<UTPSGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UTPSCommonLocalPlayer>(ExistingPlayer));

	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UTPSGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
}
