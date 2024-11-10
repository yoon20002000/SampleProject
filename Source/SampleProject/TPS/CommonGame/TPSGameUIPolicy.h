// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"

#include "TPSGameUIPolicy.generated.h"

class UCommonLocalPlayer;
class UTPSGameUIManagerSubsystem;
class ULocalPlayer;
class UTPSPrimaryGameLayout;

/**
 * 
 */
UENUM()
enum class ELocalMultiplayerInteractionMode : uint8
{
	// Fullscreen viewport for the primary player only, regardless of the other player's existence
	PrimaryOnly,

	// Fullscreen viewport for one player, but players can swap control over who's is displayed and who's is dormant
	SingleToggle,

	// Viewports displayed simultaneously for both players
	Simultaneous
};

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UTPSPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UTPSPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};

UCLASS(Abstract, Blueprintable, Within = GameUIManagerSubsystem)
class SAMPLEPROJECT_API UTPSGameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	template <typename GameUIPolicyClass = UTPSGameUIPolicy>
	static GameUIPolicyClass* GetGameUIPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<GameUIPolicyClass>(GetGameUIPolicy(WorldContextObject));
	}

	static UTPSGameUIPolicy* GetGameUIPolicy(const UObject* WorldContextObject);

public:
	virtual UWorld* GetWorld() const override;
	UTPSGameUIManagerSubsystem* GetOwningUIManager() const;
	UTPSPrimaryGameLayout* GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const;

	ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const { return LocalMultiplayerInteractionMode; }

	void RequestPrimaryControl(UTPSPrimaryGameLayout* Layout);

protected:
	void AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);

	virtual void OnRootLayoutAddedToViewport(UCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);
	virtual void OnRootLayoutRemovedFromViewport(UCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);
	virtual void OnRootLayoutReleased(UCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);

	void CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer);
	TSubclassOf<UTPSPrimaryGameLayout> GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer);

private:
	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UTPSPrimaryGameLayout> LayoutClass;

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

private:
	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	friend class UTPSGameUIManagerSubsystem;
};
