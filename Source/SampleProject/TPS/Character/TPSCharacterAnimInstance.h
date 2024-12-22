// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSCharacterAnimInstance.generated.h"

class UTPSActionComponent;
class UTPSHealthComponent;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UTPSHealthComponent> AttributeComp;

	UPROPERTY(BlueprintReadOnly)
	bool bIsAlive ;

	void UpdateIsAlive(const UTPSHealthComponent* Attribute);
};
