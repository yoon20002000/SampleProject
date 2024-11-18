// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSUIManager.generated.h"

class UCommonActivatableWidget;
class UUIDataAsset;
class UTPSGameUIPolicy;
class UTPSCommonLocalPlayer;
/**
 * 
 */

UENUM(Blueprintable)
enum class EUILayerType : uint8
{
	NONE = 0,
	GameLayer,
	GameMenu,
	Menu,
	Modal,
};

UCLASS(config = Game)
class SAMPLEPROJECT_API UTPSUIManager : public UObject
{
	GENERATED_BODY()
public:
	virtual void Initialize();
	virtual void Deinitialize();

	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	
	const UTPSGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UTPSGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }
	
	virtual void NotifyPlayerAdded(UTPSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UTPSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UTPSCommonLocalPlayer* LocalPlayer);

	void LoadUI(const FString& UIName, EUILayerType LayerType = EUILayerType::GameLayer);
	FString GetLayerNameByLayerType(const EUILayerType InLayerType);
	
protected:
	void SwitchToPolicy(UTPSGameUIPolicy* InPolicy);
	void RemoveUI(UCommonActivatableWidget* InRemoveWidget);
private:
	UPROPERTY(Transient)
	TObjectPtr<UTPSGameUIPolicy> CurrentPolicy = nullptr;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UTPSGameUIPolicy> DefaultUIPolicyClass;

	UPROPERTY(Transient)
	TObjectPtr<UUIDataAsset> UIDataAsset = nullptr;

	UPROPERTY(config, EditAnywhere)
	TSoftObjectPtr<UUIDataAsset> UIDataAssetClass;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UCommonActivatableWidget>> LoadedUIs;
};
