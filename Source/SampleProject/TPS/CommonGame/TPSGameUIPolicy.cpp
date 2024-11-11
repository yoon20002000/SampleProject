// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGameUIPolicy.h"

#include "LogTPSGame.h"
#include "TPSCommonLocalPlayer.h"
#include "TPSPrimaryGameLayout.h"
#include "TPSSystemManager.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Game/TPSGameInstance.h"
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

UWorld* UTPSGameUIPolicy::GetWorld() const
{
	/*if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetOuter()))
	{
		return GameInstance->GetWorld();
	}*/
	UWorld* World = GetOwningUIManager()->GetOuter()->GetWorld();

	return World;
}

UTPSPrimaryGameLayout* UTPSGameUIPolicy::GetRootLayout(const UTPSCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

void UTPSGameUIPolicy::NotifyPlayerAdded(UTPSCommonLocalPlayer* LocalPlayer)
{
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this](UTPSCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
		{
			NotifyPlayerRemoved(LocalPlayer);

			if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
			{
				AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
				LayoutInfo->bAddedToViewport = true;
			}
			else
			{
				CreateLayoutWidget(LocalPlayer);
			}
		});

	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UTPSGameUIPolicy::NotifyPlayerRemoved(UTPSCommonLocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = false;

		if (LocalMultiplayerInteractionMode == ELocalMultiplayerInteractionMode::SingleToggle && !LocalPlayer->IsPrimaryPlayer())
		{
			UTPSPrimaryGameLayout* RootLayout = LayoutInfo->RootLayout;
			if (RootLayout && !RootLayout->IsDormant())
			{
				// We're removing a secondary player's root while it's in control - transfer control back to the primary player's root
				RootLayout->SetIsDormant(true);
				for (const FRootViewportLayoutInfo& RootLayoutInfo : RootViewportLayouts)
				{
					if (RootLayoutInfo.LocalPlayer->IsPrimaryPlayer())
					{
						if (UTPSPrimaryGameLayout* PrimaryRootLayout = RootLayoutInfo.RootLayout)
						{
							PrimaryRootLayout->SetIsDormant(false);
						}
					}
				}
			}
		}
	}
}

void UTPSGameUIPolicy::NotifyPlayerDestroyed(UTPSCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);
	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		UTPSPrimaryGameLayout* Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIdx);

		RemoveLayoutFromViewport(LocalPlayer, Layout);

		OnRootLayoutReleased(LocalPlayer, Layout);
	}
}

void UTPSGameUIPolicy::AddLayoutToViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout)
{
	UE_LOG(LogTPSGame, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);

	OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}

void UTPSGameUIPolicy::RemoveLayoutFromViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		UE_LOG(LogTPSGame, Log, TEXT("[%s] is removing player [%s]'s root layout [%s] from the viewport"), *GetName(), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

		Layout->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogTPSGame, Log, TEXT("Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Noting in case we leak it."), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
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

void UTPSGameUIPolicy::OnRootLayoutRemovedFromViewport(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout)
{

}

void UTPSGameUIPolicy::OnRootLayoutReleased(UTPSCommonLocalPlayer* LocalPlayer, UTPSPrimaryGameLayout* Layout)
{

}

void UTPSGameUIPolicy::RequestPrimaryControl(UTPSPrimaryGameLayout* Layout)
{
	if (LocalMultiplayerInteractionMode == ELocalMultiplayerInteractionMode::SingleToggle && Layout->IsDormant())
	{
		for (const FRootViewportLayoutInfo& LayoutInfo : RootViewportLayouts)
		{
			UTPSPrimaryGameLayout* RootLayout = LayoutInfo.RootLayout;
			if (RootLayout && !RootLayout->IsDormant())
			{
				RootLayout->SetIsDormant(true);
				break;
			}
		}
		Layout->SetIsDormant(false);
	}
}

void UTPSGameUIPolicy::CreateLayoutWidget(UTPSCommonLocalPlayer* LocalPlayer)
{

	UWorld* world = GetWorld();
	APlayerController* PlayerController = LocalPlayer->GetPlayerController(world);

	if (PlayerController != nullptr)
	{
		TSubclassOf<UTPSPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UTPSPrimaryGameLayout* NewLayoutObject = CreateWidget<UTPSPrimaryGameLayout>(PlayerController, LayoutWidgetClass);
			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UTPSPrimaryGameLayout> UTPSGameUIPolicy::GetLayoutWidgetClass(UTPSCommonLocalPlayer* LocalPlayer)
{
	return LayoutClass.LoadSynchronous();
}
