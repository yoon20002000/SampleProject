// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"

#include "TPSGameUIManagerSubsystem.generated.h"

class FSubsystemCollectionBase;
class UCommonLocalPlayer;
class UTPSGameUIPolicy;
class UObject;

/**
 * This manager is intended to be replaced by whatever your game needs to
 * actually create, so this class is abstract to prevent it from being created.
 * 
 * If you just need the basic functionality you will start by sublcassing this
 * subsystem in your own game.
 */
UCLASS(Abstract, config = Game)
class SAMPLEPROJECT_API UTPSGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UTPSGameUIManagerSubsystem() { }
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	const UTPSGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UTPSGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

	virtual void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

protected:
	void SwitchToPolicy(UTPSGameUIPolicy* InPolicy);

private:
	UPROPERTY(Transient)
	TObjectPtr<UTPSGameUIPolicy> CurrentPolicy = nullptr;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UTPSGameUIPolicy> DefaultUIPolicyClass;
};
