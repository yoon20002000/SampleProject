// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TPSAIController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SAMPLEPROJECT_API ATPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName StartLocationKey;
	static const FName PatrolLocationKey;
	static const FName TargetActorKey;

private:
	UPROPERTY(EditDefaultsOnly,  meta=(AllowPrivateAccess=true))
	TObjectPtr<UBehaviorTree> BTAsset;
	UPROPERTY(EditDefaultsOnly,  meta=(AllowPrivateAccess=true))
	TObjectPtr<UBlackboardData> BBAsset;
};
