// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSCommonLocalPlayer.h"

#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "TPSGameUIPolicy.h"
#include "Game/TPSGameInstance.h"
#include "UI/TPSUIManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TPSCommonLocalPlayer)

class APawn;
class APlayerState;
class FViewport;
struct FSceneViewProjectionData;

UTPSCommonLocalPlayer::UTPSCommonLocalPlayer()
	: Super(FObjectInitializer::Get())
{
}

FDelegateHandle UTPSCommonLocalPlayer::CallAndRegister_OnPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());

	if (PC)
	{
		Delegate.Execute(this, PC);
	}

	return OnPlayerControllerSet.Add(Delegate);
}

FDelegateHandle UTPSCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;

	if (PlayerState)
	{
		Delegate.Execute(this, PlayerState);
	}
	
	return OnPlayerStateSet.Add(Delegate);
}

FDelegateHandle UTPSCommonLocalPlayer::CallAndRegister_OnPlayerPawnSet(FPlayerPawnSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APawn* Pawn = PC ? PC->GetPawn() : nullptr;

	if (Pawn)
	{
		Delegate.Execute(this, Pawn);
	}

	return OnPlayerPawnSet.Add(Delegate);
}

bool UTPSCommonLocalPlayer::GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const
{
	if (!bIsPlayerViewEnabled)
	{
		return false;
	}

	return Super::GetProjectionData(Viewport, ProjectionData, StereoViewIndex);
}

UTPSPrimaryGameLayout* UTPSCommonLocalPlayer::GetRootUILayout() const
{
	if (UTPSGameInstance* GameInstance = CastChecked<UTPSGameInstance>(GetGameInstance()))
	{
		if (UTPSUIManager* UIManager = GameInstance->GetUIManager())
		{
			if (UTPSGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				return Policy->GetRootLayout(this);
			}	
		}
	}

	return nullptr;
}
