// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"

#include "TPSGameUIPolicy.generated.h"

class UTPSUIManager;
class UTPSCommonLocalPlayer;
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

UCLASS(Abstract, Blueprintable, Within = TPSGameInstance)
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
	UTPSUIManager* GetOwningUIManager() const;
	UTPSPrimaryGameLayout* GetRootLayout(const UTPSCommonLocalPlayer* LocalPlayer) const;

	ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const { return LocalMultiplayerInteractionMode; }

	void RequestPrimaryControl(UTPSPrimaryGameLayout* Layout);

protected:
	void AddLayoutToViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);

	virtual void OnRootLayoutAddedToViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);
	virtual void OnRootLayoutRemovedFromViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);
	virtual void OnRootLayoutReleased(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout);

	void CreateLayoutWidget(UTPSCommonLocalPlayer* LocalPlayer);
	TSubclassOf<UTPSPrimaryGameLayout> GetLayoutWidgetClass(UTPSCommonLocalPlayer* LocalPlayer);

private:
	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UTPSPrimaryGameLayout> LayoutClass;

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

private:
	void NotifyPlayerAdded(UTPSCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UTPSCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UTPSCommonLocalPlayer* LocalPlayer);

	friend class UTPSUIManager;
};
