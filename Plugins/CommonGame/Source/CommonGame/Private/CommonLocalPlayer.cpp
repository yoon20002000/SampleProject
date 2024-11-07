// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonLocalPlayer.h"

#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"

UYCommonLocalPlayer::UYCommonLocalPlayer() : Super(FObjectInitializer::Get())
{
}

FDelegateHandle UYCommonLocalPlayer::CallAndRegister_OnPlayerControllerSet(
	FPlayerControllerSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());

	if (PC != nullptr)
	{
		Delegate.Execute(this, PC);
	}
	return OnPlayerControllerSet.Add(Delegate);
}

FDelegateHandle UYCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APlayerState* PlayerState = PC != nullptr ? PC->PlayerState : nullptr;
	if (PlayerState != nullptr)
	{
		Delegate.Execute(this, PlayerState);
	}
	return OnPlayerStateSet.Add(Delegate);
}

FDelegateHandle UYCommonLocalPlayer::CallAndRegister_OnPlayerPawnSet(FPlayerPawnSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APawn* Pawn = PC != nullptr ? PC->GetPawn() : nullptr;

	if (PC != nullptr)
	{
		Delegate.Execute(this, Pawn);
	}
	return OnPlayerPawnSet.Add(Delegate);
}

bool UYCommonLocalPlayer::GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData,
	int32 StereoViewIndex) const
{
	if(bIsPlayerViewEnabled == false)
	{
		return false;
	}
	return Super::GetProjectionData(Viewport, ProjectionData, StereoViewIndex);
}

UYPrimaryGameLayout* UYCommonLocalPlayer::GetRootUILayout() const
{
	if(UYGameUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UYGameUIManagerSubsystem>())
	{
		if(UYGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			return Policy->GetRootLayout(this);
		}
	}
	return nullptr;
}
