// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "UIDataAsset.generated.h"

class UUserWidget;
USTRUCT()
struct FUIDataAssetInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FName AssetName;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UUserWidget> UserWidgetClass;
};
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UUIDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UUserWidget* LoadUserWidget(const FName& UIName) const;
private:
	UPROPERTY(EditAnywhere)
	TArray<FUIDataAssetInfo> DataAssets;
};
