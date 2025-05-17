// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TPSAIController.generated.h"

class UTPSNameplateSource;
/**
 * 
 */
UCLASS(Blueprintable)
class SAMPLEPROJECT_API ATPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnPossess(APawn* InPawn) override;
	void RunAI(const AActor* ThisActor);
	void StopAI() const;

public:
	static const FName StartLocationKey;
	static const FName PatrolLocationKey;
	static const FName TargetActorKey;
	static const FName AIAttackRange;
	static const FName IsInCombat;
	static const FName ThreatLevel;
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UBehaviorTree> BTAsset;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UBlackboardData> BBAsset;
};
