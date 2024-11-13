// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSUIManager.h"

#include "CommonUIExtensions.h"
#include "GameplayTagsManager.h"
#include "TPSGameUIPolicy.h"

#include "Blueprint/UserWidget.h"
#include "Data/UIDataAsset.h"


void UTPSUIManager::Initialize()
{
	if (CurrentPolicy == nullptr && DefaultUIPolicyClass.IsNull() == false)
	{
		DefaultUIPolicyClass = DefaultUIPolicyClass.LoadSynchronous();
		if (DefaultUIPolicyClass.IsValid() == true)
		{
			if (UTPSGameUIPolicy* GameUIPolicy = NewObject<UTPSGameUIPolicy>(this, DefaultUIPolicyClass.Get()))
			{
				SwitchToPolicy(GameUIPolicy);
			}
		}
	}

	if (UIDataAssetClass.IsNull() == false)
	{
		UIDataAssetClass = UIDataAssetClass.LoadSynchronous();
		if (UIDataAssetClass.IsValid() == true)
		{
			UIDataAsset = NewObject<UUIDataAsset>(this,UIDataAssetClass.Get());
		}
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

void UTPSUIManager::LoadUI(const FString& UIName) 
{
	if (UIDataAsset == nullptr)
	{
		return;
	}

	UUserWidget* loadWidget = UIDataAsset->LoadUserWidget(UIName);

	if (loadWidget != nullptr)
	{
		// stack에 넣어야 됨.
		// Userwidget->AddToViewport();

		FString LayerName = GetLayerNameByLayerType(EUILayerType::GameLayer);

		//commonui 이렇게 만들어서 리스트에
		//const auto LoadUIPtr = UCommonUIExtensions::PushContentToLayer(UGameplayTagsManager::Get().RequestGameplayTag(*LayerName), loadWidget);
		//UILists.Add(LoadUIPtr);
	}
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
