// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSUIManager.generated.h"

class UUIDataAsset;
class UTPSGameUIPolicy;
class UTPSCommonLocalPlayer;
/**
 * 
 */
UCLASS(config = Game)
class SAMPLEPROJECT_API UTPSUIManager : public UObject
{
	GENERATED_BODY()
public:

	virtual void Initialize();
	virtual void Deinitialize();
	
	const UTPSGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UTPSGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }
	
	virtual void NotifyPlayerAdded(UTPSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UTPSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UTPSCommonLocalPlayer* LocalPlayer);

	void LoadUI(const FString& UIName) const;
protected:
	void SwitchToPolicy(UTPSGameUIPolicy* InPolicy);
private:
	UPROPERTY(Transient)
	TObjectPtr<UTPSGameUIPolicy> CurrentPolicy = nullptr;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UTPSGameUIPolicy> DefaultUIPolicyClass;

	UPROPERTY(Transient)
	TObjectPtr<UUIDataAsset> UIDataAsset = nullptr;
	UPROPERTY(Transient)
	TSoftClassPtr<UUIDataAsset> UIDataAssetClass;
	
};
