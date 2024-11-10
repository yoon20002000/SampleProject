// Copyright Epic Games, Inc. All Rights Reserved.


#include "CommonGameInstance.h"

#include "CommonLocalPlayer.h"
#include "CommonUISettings.h"
#include "TPSGameUIManagerSubsystem.h"

#include "ICommonUIModule.h"
#include "LogCommonGame.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonGameInstance)

UCommonGameInstance::UCommonGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		if (!PrimaryPlayer.IsValid())
		{
			UE_LOG(LogCommonGame, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"), *NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}
		
		GetSubsystem<UTPSGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		//TODO: do we want to fall back to another player?
		PrimaryPlayer.Reset();
		UE_LOG(LogCommonGame, Log, TEXT("RemoveLocalPlayer: Unsetting Primary Player from %s"), *ExistingPlayer->GetName());
	}
	GetSubsystem<UTPSGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UCommonLocalPlayer>(ExistingPlayer));

	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UCommonGameInstance::Init()
{
	Super::Init();

	// After subsystems are initialized, hook them together
	FGameplayTagContainer PlatformTraits = ICommonUIModule::GetSettings().GetPlatformTraits();
}

void UCommonGameInstance::ReturnToMainMenu()
{
	// By default when returning to main menu we should reset everything
	Super::ReturnToMainMenu();
}

void UCommonGameInstance::ResetGameAndJoinRequestedSession()
{
	// Default behavior is to return to the main menu.  The game must call JoinRequestedSession when the game is in a ready state.
	ReturnToMainMenu();
}


//void UCommonGameInstance::OnPreLoadMap(const FString& MapName)
//{
//	if (!IsDedicatedServerInstance())
//	{
//		if (!bWasInLoadMap)
//		{
//			UGameUIManagerSubsystem* UIManager = GetSubsystem<UGameUIManagerSubsystem>();
//			for (ULocalPlayer* LocalPlayer : LocalPlayers)
//			{
//				UIManager->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(LocalPlayer));
//			}
//		}
//	}
//}
