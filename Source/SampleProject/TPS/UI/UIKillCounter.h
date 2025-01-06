// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSCommonUserWidget.h"
#include "Character/TPSPlayerState.h"
#include "UIKillCounter.generated.h"


class UCommonNumericTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UUIKillCounter : public UTPSCommonUserWidget
{
	GENERATED_BODY()

public:
	void InitKillCount(ATPSPlayer* Player);
	void UninitKillCount(ATPSPlayer* Player);
	void UpdateKillCount(int32 InValue);
	
private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnUpdateKillCount(ATPSPlayerState* PlayerState, uint32 KillCount, uint8 Delta);

private:
	UPROPERTY(EditAnywhere, Blueprintable, meta=(BindWidget))
	TObjectPtr<UCommonNumericTextBlock> KillCountText;
};
