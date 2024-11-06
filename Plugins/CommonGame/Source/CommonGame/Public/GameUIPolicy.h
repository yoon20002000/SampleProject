// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"

#include "GameUIPolicy.generated.h"

class UYGameUIManagerSubsystem;
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
public:
	template<typename GameUIPolicyClass = UYGameUIPolicy>
	static GameUIPolicyClass* GetGameUIPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<UYGameUIPolicy>(GetGameUIPolicy(WorldContextObject));
	}
	static UYGameUIPolicy* GetGameUIPolicy(const UObject* WorldContextObject);
	
public:
	virtual UWorld* GetWorld() const override;
	UYGameUIManagerSubsystem* GetOwningUIManager() const;
	UYPrimaryGameLayout* GetRootLayout(const UYCommonLocalPlayer* LocalPlayer) const;

	ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const
	{
		return LocalMultiplayerInteractionMode;
	}
	void RequestPrimaryControl(UYPrimaryGameLayout* Layout);

protected:
	void AddLayoutToViewport(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout);

	virtual void OnRootLayoutAddedToViewport(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout);
	virtual void OnRootLayoutRemovedFromViewport(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout);
	virtual void OnRootLayoutReleased(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout);

	void CreateLayoutWidget(UYCommonLocalPlayer* LocalPlayer);
	TSubclassOf<UYPrimaryGameLayout> GetLayoutWidgetClass(UYCommonLocalPlayer* LocalPlayer) const;
	
private:
	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;
	
	UPROPERTY(Transient)
	TSoftClassPtr<UYPrimaryGameLayout> LayoutClass;
	
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayoutInfos;
	
private:
	void NotifyPlayerAdded(UYCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UYCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UYCommonLocalPlayer* LocalPlayer);

	friend class UYGameUIManagerSubsystem;
};
