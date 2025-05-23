// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "TPSCommonUserWidget.generated.h"

class ATPSPlayer;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSCommonUserWidget : public UCommonUserWidget
{
	GENERATED_BODY()
protected:
	ATPSPlayer* GetTPSPlayer() const;
};
