// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGameUIPolicy.h"

#include "LogTPSGame.h"
#include "TPSPrimaryGameLayout.h"
#include "TPSSystemManager.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Game/TPSCommonLocalPlayer.h"
#include "UI/TPSUIManager.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TPSGameUIPolicy)

// Static
UTPSGameUIPolicy* UTPSGameUIPolicy::GetGameUIPolicy(const UObject* WorldContextObject)
{
	UTPSSystemManager* Manager = UTPSSystemManager::Get();
	if (UTPSUIManager* UIManager = Manager->GetUIManager())
	{
		if (UTPSGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			return UIManager->GetCurrentUIPolicy();
		}
	}

	return nullptr;
}

UTPSUIManager* UTPSGameUIPolicy::GetOwningUIManager() const
{
	UTPSSystemManager* Manager = UTPSSystemManager::Get();

	return Manager->GetUIManager();
}

UTPSPrimaryGameLayout* UTPSGameUIPolicy::GetRootLayout() const
{
	return RootViewportLayout.RootLayout;
}

UWorld* UTPSGameUIPolicy::GetWorld() const
{
	return UTPSSystemManager::Get()->GetWorld();
}

void UTPSGameUIPolicy::NotifyPlayerAdded(UTPSCommonLocalPlayer* LocalPlayer)
{
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(
		this, [this](UTPSCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
		{
			NotifyPlayerRemoved(LocalPlayer);

			if (UTPSPrimaryGameLayout* layout = GetRootLayout())
			{
				AddLayoutToViewport(LocalPlayer, layout);
				RootViewportLayout.bAddedToViewport = true;
			}
			else
			{
				CreateLayoutWidget(LocalPlayer);
			}
		});

	if (RootViewportLayout.RootLayout != nullptr)
	{
		AddLayoutToViewport(LocalPlayer, RootViewportLayout.RootLayout);
		RootViewportLayout.bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UTPSGameUIPolicy::NotifyPlayerRemoved(UTPSCommonLocalPlayer* LocalPlayer)
{
	RemoveLayoutFromViewport(LocalPlayer, RootViewportLayout.RootLayout);
	RootViewportLayout.bAddedToViewport = false;

	if (LocalMultiplayerInteractionMode == ELocalMultiplayerInteractionMode::SingleToggle && !LocalPlayer->
		IsPrimaryPlayer())
	{
		UTPSPrimaryGameLayout* RootLayout = RootViewportLayout.RootLayout;
		if (RootLayout && !RootLayout->IsDormant())
		{
			RootLayout->SetIsDormant(true);
		}
	}
}

void UTPSGameUIPolicy::NotifyPlayerDestroyed(UTPSCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);


	RemoveLayoutFromViewport(LocalPlayer, RootViewportLayout.RootLayout);
	OnRootLayoutReleased(LocalPlayer, RootViewportLayout.RootLayout);
	RootViewportLayout.Reset();
}

void UTPSGameUIPolicy::AddLayoutToViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout)
{
	UE_LOG(LogTPSGame, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(),
	       *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);

	OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}

void UTPSGameUIPolicy::RemoveLayoutFromViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout)
{
	if (Layout == nullptr)
	{
		return;
	}
	
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		UE_LOG(LogTPSGame, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"), *GetName(),
		       *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

		Layout->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogTPSGame, Log,
			       TEXT(
				       "Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."
			       ), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
		}

		OnRootLayoutRemovedFromViewport(LocalPlayer, Layout);
	}
}

void UTPSGameUIPolicy::OnRootLayoutAddedToViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout)
{
#if WITH_EDITOR
	if (GIsEditor && LocalPlayer->IsPrimaryPlayer())
	{
		// So our controller will work in PIE without needing to click in the viewport
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}
#endif
}

void UTPSGameUIPolicy::OnRootLayoutRemovedFromViewport(UTPSCommonLocalPlayer* LocalPlayer,
                                                       UTPSPrimaryGameLayout* Layout)
{
}

void UTPSGameUIPolicy::OnRootLayoutReleased(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout)
{
}

void UTPSGameUIPolicy::RequestPrimaryControl(UTPSPrimaryGameLayout* Layout)
{
	if (LocalMultiplayerInteractionMode == ELocalMultiplayerInteractionMode::SingleToggle && Layout->IsDormant())
	{
		UTPSPrimaryGameLayout* RootLayout = RootViewportLayout.RootLayout;
		if (RootLayout && !RootLayout->IsDormant())
		{
			RootLayout->SetIsDormant(true);
		}
		Layout->SetIsDormant(false);
	}
}

void UTPSGameUIPolicy::CreateLayoutWidget(UTPSCommonLocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UTPSPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UTPSPrimaryGameLayout* NewLayoutObject = CreateWidget<UTPSPrimaryGameLayout>(
				PlayerController, LayoutWidgetClass);
			RootViewportLayout.RootLayout = NewLayoutObject;
			RootViewportLayout.bAddedToViewport = true;

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UTPSPrimaryGameLayout> UTPSGameUIPolicy::GetLayoutWidgetClass(UTPSCommonLocalPlayer* LocalPlayer)
{
	return LayoutClass.LoadSynchronous();
}
