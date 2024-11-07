// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LocalPlayer.h"
#include "CommonLocalPlayer.generated.h"

class UYPrimaryGameLayout;
class APawn;
class APlayerController;
class APlayerState;
class FViewport;
class UObject;

/**
 * 
 */
UCLASS()
class COMMONGAME_API UYCommonLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
public:
	UYCommonLocalPlayer();
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UYCommonLocalPlayer* UYCommonLocalPlayer, APlayerController* PlayerController);
	FPlayerControllerSetDelegate OnPlayerControllerSet;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, UYCommonLocalPlayer* UYCommonLocalPlayer, APlayerState* PlayerState);
	FPlayerStateSetDelegate OnPlayerStateSet;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerPawnSetDelegate, UYCommonLocalPlayer* UYCommonLocalPlayer, APawn* Pawn);
	FPlayerPawnSetDelegate OnPlayerPawnSet;

	FDelegateHandle CallAndRegister_OnPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate Delegate);
	FDelegateHandle CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate);
	FDelegateHandle CallAndRegister_OnPlayerPawnSet(FPlayerPawnSetDelegate::FDelegate Delegate);
public:
	virtual bool GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const override;

	bool IsPlayerViewEnabled() const
	{
		return bIsPlayerViewEnabled;
	}
	void SetIsPlayerViewEnabled(bool bInIsPlayerViewEnabled)
	{
		bIsPlayerViewEnabled = bInIsPlayerViewEnabled;
	}
	UYPrimaryGameLayout* GetRootUILayout()const;
	
private:
	bool bIsPlayerViewEnabled = true;	
};
