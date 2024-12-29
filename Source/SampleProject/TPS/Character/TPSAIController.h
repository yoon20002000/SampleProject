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
	virtual void OnUnPossess() override;
	
	virtual void Tick(float DeltaSeconds) override;
	bool IsAlive() const;
protected:
	virtual void BeginPlay() override;

private:
	void OnRepeatTimer();

	
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> AIBT;
	
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
