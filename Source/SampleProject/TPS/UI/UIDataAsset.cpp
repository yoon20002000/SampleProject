// Fill out your copyright notice in the Description page of Project Settings.


#include "UIDataAsset.h"

#include "Blueprint/UserWidget.h"

UUserWidget* UUIDataAsset::LoadUserWidget(const FString& UIName) const
{
	auto UIDataAssetInfo = DataAssets.FindByPredicate([&UIName](const FUIDataAssetInfo& UIDataAssetInfo)
	{
		return UIDataAssetInfo.AssetName == UIName;
		
	});
	if (UIDataAssetInfo != nullptr)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), UIDataAssetInfo->UserWidgetClass.LoadSynchronous());
		
		return WidgetInstance;	
	}
	else
	{
		return nullptr;
	}
}

