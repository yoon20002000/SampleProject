// Fill out your copyright notice in the Description page of Project Settings.


#include "UIDataAsset.h"

#include "TPSHelper.h"
#include "Blueprint/UserWidget.h"

UUserWidget* UUIDataAsset::LoadUserWidget(const FName& UIName) const
{
	auto UIDataAssetInfo = DataAssets.FindByPredicate([&UIName](const FUIDataAssetInfo& InUIDataAssetInfo)
	{
		return InUIDataAssetInfo.AssetName == UIName;
		
	});
	if (UIDataAssetInfo != nullptr)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(TPSHelper::GetWorld(), UIDataAssetInfo->UserWidgetClass.LoadSynchronous());
		
		return WidgetInstance;	
	}
	else
	{
		return nullptr;
	}
}

