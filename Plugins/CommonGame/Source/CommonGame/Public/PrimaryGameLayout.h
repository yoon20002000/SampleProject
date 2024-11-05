// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CommonUserWidget.h"
#include "PrimaryGameLayout.generated.h"

class APlayerController;
class UClass;
class UCommonActivatableWidgetContainerBase;
class ULocalPlayer;
class UObject;
struct FFrame;

/**
 * UI load state operation
 */
enum class EAsyncWidgetLayerState:uint8
{
	Canceled,
	Initialize,
	AfterPush
};

/**
 * 기본 게임 UI Layout
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class COMMONGAME_API UYPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	static UYPrimaryGameLayout* GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject);
	static UYPrimaryGameLayout* GetPrimaryGameLayout(APlayerController* PlayerController);
	static UYPrimaryGameLayout* GetPrimaryGameLayout(ULocalPlayer* LocalPlayer);
	
};
