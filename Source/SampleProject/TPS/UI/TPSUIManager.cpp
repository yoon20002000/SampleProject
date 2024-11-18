// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSUIManager.h"

#include "CommonActivatableWidget.h"
#include "CommonUIExtensions.h"
#include "GameplayTagsManager.h"
#include "TPSGameManager.h"
#include "TPSGameUIPolicy.h"
#include "TPSSystemManager.h"

#include "Blueprint/UserWidget.h"
#include "Data/UIDataAsset.h"


void UTPSUIManager::Initialize()
{
	if (CurrentPolicy == nullptr && DefaultUIPolicyClass.IsNull() == false)
	{
		const UClass* LoadedClass = DefaultUIPolicyClass.LoadSynchronous();
		if (LoadedClass != nullptr)
		{
			if (UTPSGameUIPolicy* GameUIPolicy = NewObject<UTPSGameUIPolicy>(this, LoadedClass))
			{
				SwitchToPolicy(GameUIPolicy);
			}
		}
	}

	if (UIDataAssetClass.IsNull() == false)
	{
		UIDataAsset = UIDataAssetClass.LoadSynchronous();
	}
}

void UTPSUIManager::Deinitialize()
{
	SwitchToPolicy(nullptr);
}

void UTPSUIManager::BeginPlay()
{
	
}

void UTPSUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void UTPSUIManager::NotifyPlayerAdded(UTPSCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UTPSUIManager::NotifyPlayerRemoved(UTPSCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UTPSUIManager::NotifyPlayerDestroyed(UTPSCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}

void UTPSUIManager::LoadUI(const FString& UIName, EUILayerType LayerType)
{
	if (UIDataAsset == nullptr)
	{
		return;
	}

	UUserWidget* loadWidget = UIDataAsset->LoadUserWidget(UIName);

	if (loadWidget != nullptr)
	{
		FString LayerName = GetLayerNameByLayerType(LayerType);

		const TObjectPtr<UCommonActivatableWidget> LoadUIPtr = UCommonUIExtensions::PushContentToLayer(
			UGameplayTagsManager::Get().RequestGameplayTag(*LayerName),
			CastChecked<UCommonActivatableWidget>(loadWidget)->GetClass());
		LoadedUIs.Add(LoadUIPtr);
	}
}

void UTPSUIManager::RemoveUI(UCommonActivatableWidget* InRemoveWidget)
{
	UCommonUIExtensions::PopContentFromLayer(InRemoveWidget);
	LoadedUIs.Remove(InRemoveWidget);
}

FString UTPSUIManager::GetLayerNameByLayerType(const EUILayerType InLayerType)
{
	FString LayerName;

	switch (InLayerType)
	{
	case EUILayerType::GameLayer:
		LayerName = TEXT("UI.Layer.Game");
		break;
	case EUILayerType::GameMenu:
		LayerName = TEXT("UI.Layer.GameMenu");
		break;
	case EUILayerType::Menu:
		LayerName = TEXT("UI.Layer.Menu");
		break;
	case EUILayerType::Modal:
		LayerName = TEXT("UI.Layer.Modal");
		break;
	default:
		break;
	}

	return LayerName;
}

void UTPSUIManager::SwitchToPolicy(UTPSGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}
