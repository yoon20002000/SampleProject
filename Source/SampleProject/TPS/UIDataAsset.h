// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "UIDataAsset.generated.h"


USTRUCT()
struct FUIDataAssetInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FString AssetName;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCommonActivatableWidget> ActivatableWidgetClass;
};
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UUIDataAsset : public UDataAsset
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	TArray<FUIDataAssetInfo> DataAssets;
};
