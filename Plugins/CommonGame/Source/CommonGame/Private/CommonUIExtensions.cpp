// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonUIExtensions.h"

#include "CommonActivatableWidget.h"
#include "CommonInputSubsystem.h"
#include "GameplayTagContainer.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"

int32 UYCommonUIExtensions::InputSuspensions = 0;

UCommonActivatableWidget* UYCommonUIExtensions::PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer,
	FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	if(ensure(LocalPlayer) == false || ensure(WidgetClass) == false)
	{
		return nullptr;
	}

	if(UYGameUIManagerSubsystem* UIManager = LocalPlayer -> GetGameInstance()->GetSubsystem<UYGameUIManagerSubsystem>())
	{
		if(UYGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if(UYPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UYCommonLocalPlayer>(LocalPlayer)))
			{
				return RootLayout->PushWidgetToLayerStack(LayerName, WidgetClass);
			}
		}
	}
	return nullptr;
}

void UYCommonUIExtensions::PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	if(ActivatableWidget == nullptr)
	{
		return;
	}
	if(const ULocalPlayer* LocalPlayer = ActivatableWidget->GetOwningLocalPlayer())
	{
		if(const UYGameUIManagerSubsystem* UIManager =LocalPlayer->GetGameInstance()->GetSubsystem<UYGameUIManagerSubsystem>())
		{
			if(const UYGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if(UYPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UYCommonLocalPlayer>(LocalPlayer)))
				{
					RootLayout->FindAndRemoveWidgetFromLayer(ActivatableWidget);
				}
			}
		}
	}
}

FName UYCommonUIExtensions::SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason)
{
	return SuspendInputForPlayer(PlayerController != nullptr ? PlayerController->GetLocalPlayer() : nullptr, SuspendReason);
}

FName UYCommonUIExtensions::SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason)
{
	if(UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		++InputSuspensions;
		FName SuspendToken = SuspendReason;
		SuspendToken.SetNumber(InputSuspensions);

		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, true);

		return SuspendToken;
	}
	return NAME_None;
}

void UYCommonUIExtensions::ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken)
{
	ResumeInputForPlayer(PlayerController != nullptr ? PlayerController->GetLocalPlayer() : nullptr, SuspendToken);
}

void UYCommonUIExtensions::ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken)
{
	if(SuspendToken == NAME_None)
	{
		return;
	}

	if(UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, false);
	}
}
