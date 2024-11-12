// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "TPSCommonPlayerController.generated.h"

class APawn;
class UObject;

UCLASS(config=Game)
class SAMPLEPROJECT_API ATPSCommonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPSCommonPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ReceivedPlayer() override;	
	virtual void SetPawn(APawn* InPawn) override;
	virtual void OnPossess(class APawn* APawn) override;
	virtual void OnUnPossess() override;
	
protected:
	virtual void OnRep_PlayerState() override;
};
