// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"

class FSubsystemCollectionBase;
class UYCommonLocalPlayer;
class UYGameUIPolicy;
class UObject;
/**
 * 
 */
UCLASS(Abstract, config = Game)
class COMMONGAME_API UYGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UYGameUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	const UYGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UYGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

	virtual void NotifyPlayerAdded(UYCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UYCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UYCommonLocalPlayer* LocalPlayer);
	
protected:
	void SwitchToPolicy(UYGameUIPolicy* InPolicy);
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UYGameUIPolicy> CurrentPolicy = nullptr;
	
	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UYGameUIPolicy> DefaultUIPolicyClass;
};
