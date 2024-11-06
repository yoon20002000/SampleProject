// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUIPolicy.h"

#include "GameUIManagerSubsystem.h"
#include "LogCommonGame.h"
#include "PrimaryGameLayout.h"

UYGameUIPolicy* UYGameUIPolicy::GetGameUIPolicy(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UYGameUIManagerSubsystem* UIManager = UGameInstance::GetSubsystem<UYGameUIManagerSubsystem>(
				GameInstance))
			{
				return UIManager->GetCurrentUIPolicy();
			}
		}
	}
	return nullptr;
}

UWorld* UYGameUIPolicy::GetWorld() const
{
	return GetOwningUIManager()->GetGameInstance()->GetWorld();
}

UYGameUIManagerSubsystem* UYGameUIPolicy::GetOwningUIManager() const
{
	return CastChecked<UYGameUIManagerSubsystem>(GetOuter());
}

UYPrimaryGameLayout* UYGameUIPolicy::GetRootLayout(const UYCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayoutInfos.FindByKey(LocalPlayer);
	return LayoutInfo != nullptr ? LayoutInfo->RootLayout : nullptr;
}

void UYGameUIPolicy::RequestPrimaryControl(UYPrimaryGameLayout* Layout)
{
	if (LocalMultiplayerInteractionMode == ELocalMultiplayerInteractionMode::SingleToggle && Layout->IsDormant() ==
		true)
	{
		for (const FRootViewportLayoutInfo& LayoutInfo : RootViewportLayoutInfos)
		{
			UYPrimaryGameLayout* RootLayout = LayoutInfo.RootLayout;
			if (RootLayout != nullptr && RootLayout->IsDormant() == false)
			{
				RootLayout->SetIsDormant(true);
				break;
			}
		}
		Layout->SetIsDormant(false);
	}
}

void UYGameUIPolicy::AddLayoutToViewport(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout)
{
	UE_LOG(LogCommonGame, Log, TEXT("[%s] is adding player [%s]'s root layout [%s] to the viewport"), *GetName(),
	       *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));

	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));
	Layout->AddToPlayerScreen(1000);

	OnRootLayoutAddedToViewport(LocalPlayer, Layout);
}

void UYGameUIPolicy::RemoveLayoutFromViewport(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout)
{
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid() == true)
	{
		UE_LOG(LogCommonGame, Log, TEXT("[%s] is removing player [%s]'s root [%s] from the viewport"), *GetName(),
		       *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
		Layout->RemoveFromParent();
		if (LayoutSlateWidget.IsValid())
		{
			UE_LOG(LogCommonGame, Log,
			       TEXT(
				       "Player [%s]'s root layout [%s] has been removed from the viewport, but other references to its underlying Slate widget still exist. Nothing in case we leak it."
			       ), *GetNameSafe(LocalPlayer), *GetNameSafe(Layout));
		}
		OnRootLayoutRemovedFromViewport(LocalPlayer, Layout);
	}
}

void UYGameUIPolicy::OnRootLayoutAddedToViewport(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout)
{
#if WITH_EDITOR
	if (GIsEditor == true && LocalPlayer->IsPrimaryPlayer())
	{
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}
#endif
}

void UYGameUIPolicy::OnRootLayoutRemovedFromViewport(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout)
{
}

void UYGameUIPolicy::OnRootLayoutReleased(UYCommonLocalPlayer* LocalPlayer, UYPrimaryGameLayout* Layout)
{
}

void UYGameUIPolicy::CreateLayoutWidget(UYCommonLocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UYPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass != nullptr && LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract) == false))
		{
			UYPrimaryGameLayout* NewLayoutObject = CreateWidget<UYPrimaryGameLayout>(
				PlayerController, LayoutWidgetClass);
			RootViewportLayoutInfos.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

TSubclassOf<UYPrimaryGameLayout> UYGameUIPolicy::GetLayoutWidgetClass(UYCommonLocalPlayer* LocalPlayer) const
{
	return LayoutClass.LoadSynchronous();
}

void UYGameUIPolicy::NotifyPlayerAdded(UYCommonLocalPlayer* LocalPlayer)
{
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(
		this, [this](UYCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
		{
			NotifyPlayerRemoved(LocalPlayer);
			if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayoutInfos.FindByKey(LocalPlayer))
			{
				AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
				LayoutInfo->bAddedToViewport = true;
			}
			else
			{
				CreateLayoutWidget(LocalPlayer);
			}
		});

	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayoutInfos.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UYGameUIPolicy::NotifyPlayerRemoved(UYCommonLocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayoutInfos.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = false;

		if (LocalMultiplayerInteractionMode == ELocalMultiplayerInteractionMode::SingleToggle && LocalPlayer->
			IsPrimaryPlayer() == false)
		{
			UYPrimaryGameLayout* RootLayout = LayoutInfo->RootLayout;
			if (RootLayout != nullptr && RootLayout->IsDormant() == false)
			{
				RootLayout->SetIsDormant(true);
				for (const FRootViewportLayoutInfo& RootLayoutInfo : RootViewportLayoutInfos)
				{
					if (RootLayoutInfo.LocalPlayer->IsPrimaryPlayer() == true)
					{
						if (UYPrimaryGameLayout* PrimaryRootLayout = RootLayoutInfo.RootLayout)
						{
							PrimaryRootLayout->SetIsDormant(false);
						}
					}
				}
			}
		}
	}
}

void UYGameUIPolicy::NotifyPlayerDestroyed(UYCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);
	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);
	const int32 LayoutInfoIdx = RootViewportLayoutInfos.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		UYPrimaryGameLayout* Layout = RootViewportLayoutInfos[LayoutInfoIdx].RootLayout;
		RootViewportLayoutInfos.RemoveAt(LayoutInfoIdx);

		RemoveLayoutFromViewport(LocalPlayer, Layout);
		OnRootLayoutReleased(LocalPlayer, Layout);
	}
}
