// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "TPSUIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSUIManagerSubsystem : public UYGameUIManagerSubsystem
{
	GENERATED_BODY()
public:
	UTPSUIManagerSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize()override;
private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();

	FTSTicker::FDelegateHandle TickHandle;
};
