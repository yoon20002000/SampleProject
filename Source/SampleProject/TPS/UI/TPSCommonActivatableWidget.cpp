// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSCommonActivatableWidget.h"

#include "TPSHelper.h"

void UTPSCommonActivatableWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTPSCommonActivatableWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTPSCommonActivatableWidget::SetInputUIMode(bool bShowCursor)
{
	if (APlayerController* PC = TPSHelper::GetPlayerController(GetWorld()))
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(bShowCursor);
	}
}

void UTPSCommonActivatableWidget::SetInputGameMode(bool bShowCursor)
{
	if (APlayerController* PC = TPSHelper::GetPlayerController(GetWorld()))
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(bShowCursor);
	}
}
