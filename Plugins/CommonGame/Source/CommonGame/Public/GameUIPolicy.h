// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CommonLocalPlayer.h"

#include "GameUIPolicy.generated.h"

class UYPrimaryGameLayout;
class UYCommonLocalPlayer;
/**
 * 
 */
UENUM()
enum class ELocalMultiplayerInteractionMode : uint8
{
	// Fullscreen viewport for primary player only
	PrimaryOnly,
	// Fullscreen viewport for one player
	SingleToggle,
	// Viewports displayed simultaneously for both players
	Simultaneous
};
USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UYPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FRootViewportLayoutInfo(){}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UYPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		:	LocalPlayer(InLocalPlayer),
			RootLayout(InRootLayout),
			bAddedToViewport(bIsInViewport)
	{
		
	}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const
	{
		return LocalPlayer == OtherLocalPlayer;
	}
};
UCLASS()
class COMMONGAME_API UYGameUIPolicy : public UObject
{
	GENERATED_BODY()
private:
	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;
	
	UPROPERTY(Transient)
	TSoftClassPtr<UYPrimaryGameLayout> LayoutClass;
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayoutInfos;
public:
	UYPrimaryGameLayout* GetRootLayout(const UYCommonLocalPlayer* LocalPlayer) const;
private:
	void NotifyPlayerAdded(UYCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UYCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UYCommonLocalPlayer* LocalPlayer);

	friend class UYGameUIManagerSubsystem;
};
