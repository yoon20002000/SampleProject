// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TPSGamePlayerController.h"

#include "TPSSystemManager.h"
#include "UI/TPSUIManager.h"

void ATPSGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UTPSSystemManager::Get()->GetUIManager()->LoadUI(TEXT("BattleHUD"), EUILayerType::GameLayer);
	
	// FInputModeUIOnly Mode;
	// Mode.SetWidgetToFocus(nullptr);
	// SetInputMode(Mode);
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
