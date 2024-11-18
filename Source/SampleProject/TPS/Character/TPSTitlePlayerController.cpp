// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSTitlePlayerController.h"

#include "TPSSystemManager.h"
#include "UI/TPSUIManager.h"


void ATPSTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UTPSSystemManager::Get()->GetUIManager()->LoadUI(TEXT("Title"), EUILayerType::Menu);
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
