// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryGameLayout.h"

#include "CommonLocalPlayer.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include "LogCommonGame.h"
#include "Kismet/GameplayStatics.h"

UYPrimaryGameLayout* UYPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	APlayerController* PlayerController = GameInstance->GetPrimaryPlayerController(false);
	return GetPrimaryGameLayout(PlayerController);
}

UYPrimaryGameLayout* UYPrimaryGameLayout::GetPrimaryGameLayout(APlayerController* PlayerController)
{
	return PlayerController != nullptr
		       ? GetPrimaryGameLayout(Cast<UYCommonLocalPlayer>(PlayerController->Player))
		       : nullptr;
}

UYPrimaryGameLayout* UYPrimaryGameLayout::GetPrimaryGameLayout(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer != nullptr)
	{
		const UYCommonLocalPlayer* CommonLocalPlayer = CastChecked<UYCommonLocalPlayer>(LocalPlayer);
		if (const UGameInstance* GameInstance = CommonLocalPlayer->GetGameInstance())
		{
			if(UYGameUIManagerSubsystem* UIManager = GameInstance->GetSubsystem<UYGameUIManagerSubsystem>())
			{
				if(const UYGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
				{
					if(UYPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CommonLocalPlayer))
					{
						return RootLayout;
					}
				}
			}
		}
	}
	return nullptr;
}

UYPrimaryGameLayout::UYPrimaryGameLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UYPrimaryGameLayout::SetIsDormant(bool bInDormant)
{
	if(bIsDormant != bInDormant)
	{
		const ULocalPlayer* LP = GetOwningLocalPlayer();
		const int32 PlayerId = LP ? LP->GetControllerId() : -1;
		const TCHAR* OldDormacnyStr = bIsDormant ? TEXT("Dormant") : TEXT("No Dormant");
		const TCHAR* NewDormancyStr = bInDormant ? TEXT("Dormant") : TEXT("No Dormant");
		const TCHAR* PrimaryPlayerStr = LP !=nullptr && LP->IsPrimaryPlayer() ? TEXT("[Primary]") : TEXT("[Non-Primary]");
		UE_LOG(LogCommonGame, Display, TEXT("%s PrimaryGameLayout Dormancy changed for [%d] from [%s]to [%s]"), PrimaryPlayerStr, PlayerId,OldDormacnyStr,NewDormancyStr);

		bIsDormant = bInDormant;
		OnIsDormantChanged();
	}
}

void UYPrimaryGameLayout::FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	for(const TTuple<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>>& LayerKVP : Layers)
	{
		LayerKVP.Value->RemoveWidget(*ActivatableWidget);
	}
}

UCommonActivatableWidgetContainerBase* UYPrimaryGameLayout::GetLayerWidget(FGameplayTag LayerName)
{
	return Layers.FindRef(LayerName);
}

void UYPrimaryGameLayout::RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget)
{
	if(IsDesignTime() == false)
	{
		LayerWidget->OnTransitioningChanged.AddUObject(this, &UYPrimaryGameLayout::OnWidgetStackTransitioning);
		LayerWidget->SetTransitionDuration(0.0f);

		Layers.Add(LayerTag, LayerWidget);
	}
}

void UYPrimaryGameLayout::OnIsDormantChanged()
{
	
}

void UYPrimaryGameLayout::OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget,
	bool bIsTransitioning)
{
	if(bIsTransitioning == true)
	{
		const FName SuspendToken = UYCommonUIExtensions::SuspendInputForPlayer(GetOwningLocalPlayer(), TEXT("GlobalStackTransion"));
		SuspendInputTokens.Add(SuspendToken);
	}
	else
	{
		if(ensure(SuspendInputTokens.Num() > 0))
		{
			const FName SuspendToken = SuspendInputTokens.Pop();
			UYCommonUIExtensions::ResumeInputForPlayer(GetOwningLocalPlayer(), SuspendToken);
		}
	}
}
