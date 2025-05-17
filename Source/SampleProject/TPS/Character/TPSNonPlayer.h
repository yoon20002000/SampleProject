// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TPSCharacter.h"
#include "TPSNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSNonPlayer : public ATPSCharacter
{
	GENERATED_BODY()
public:
	FORCEINLINE float GetAttackRange() const
	{
		return AttackRange;
	}
private:
	UPROPERTY(EditAnywhere)
	float AttackRange;
};
