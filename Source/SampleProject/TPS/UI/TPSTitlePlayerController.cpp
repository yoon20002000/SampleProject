// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSTitlePlayerController.h"

#include "TPSSystemManager.h"
#include "TPSUIManager.h"

void ATPSTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();


	UTPSSystemManager::Get()->GetUIManager()->LoadUI(TEXT("Title"), EUILayerType::GameMenu);

	// FInputModeUIOnly Mode;
	// Mode.SetWidgetToFocus(Widget->GetCachedWidget());
	// SetInputMode(Mode);
	// bShowMouseCursor = true;
}
