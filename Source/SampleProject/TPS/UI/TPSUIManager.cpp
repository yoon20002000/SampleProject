// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSUIManager.h"

#include "CommonGame/TPSGameUIPolicy.h"

void UTPSUIManager::Initialize()
{
	if (CurrentPolicy == nullptr && DefaultUIPolicyClass.IsNull() == false)
	{
		if (TSoftClassPtr<UTPSGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous())
		{
			if (UTPSGameUIPolicy* GameUIPolicy = NewObject<UTPSGameUIPolicy>(this, PolicyClass.Get()))
			{
				SwitchToPolicy(GameUIPolicy);
			}
		}
	}
}

void UTPSUIManager::Deinitialize()
{
	SwitchToPolicy(nullptr);
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

void UTPSUIManager::SwitchToPolicy(UTPSGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}
