// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSUIManager.h"

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

void UTPSUIManager::LoadUI(const FString& UIName) const
{
	if (UIDataAsset == nullptr)
	{
		return;
	}

	UUserWidget* Userwidget = UIDataAsset->LoadUserWidget(UIName);

	if (Userwidget != nullptr)
	{
		// stack에 넣어야 됨.
		// Userwidget->AddToViewport();
	}
}

void UTPSUIManager::SwitchToPolicy(UTPSGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}
