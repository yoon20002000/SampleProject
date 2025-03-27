// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TPSStart.h"
#include "TPSAIStart.generated.h"

class UArrowComponent;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSAIStart : public ATPSStart
{
	GENERATED_BODY()
public:
	ATPSAIStart(const FObjectInitializer& ObjectInitializer);
#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
public:
#endif
#if WITH_EDITORONLY_DATA
	/** Returns ArrowComponent subobject **/
	UArrowComponent* GetArrowComponent() const;
#endif
};
