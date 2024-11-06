// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUIPolicy.h"

UYPrimaryGameLayout* UYGameUIPolicy::GetRootLayout(const UYCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayoutInfos.FindByKey(LocalPlayer);
	return LayoutInfo != nullptr? LayoutInfo->RootLayout : nullptr;
}

void UYGameUIPolicy::NotifyPlayerAdded(UYCommonLocalPlayer* LocalPlayer)
{
	
}

void UYGameUIPolicy::NotifyPlayerRemoved(UYCommonLocalPlayer* LocalPlayer)
{
}

void UYGameUIPolicy::NotifyPlayerDestroyed(UYCommonLocalPlayer* LocalPlayer)
{
}
