// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSPauseMenuWidget.h"

#include "CommonUIExtensions.h"
#include "TPSHelper.h"
#include "Components/Button.h"

bool UTPSPauseMenuWidget::Initialize()
{
	bool bResult = Super::Initialize();

	SetMouseCursor(true);
	
	if (OptionButton != nullptr)
	{
		OptionButton->OnClicked.AddDynamic(this, &UTPSPauseMenuWidget::OnClickedOption);
	}

	if (QuitButton != nullptr)
	{
		QuitButton->OnClicked.AddDynamic(this, &UTPSPauseMenuWidget::OnClickedQuit);
	}

	if (ContinueButton != nullptr)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UTPSPauseMenuWidget::OnClickedContinue);
	}

	
	return bResult;
}

void UTPSPauseMenuWidget::OnClickedOption()
{
	UE_LOG(LogTemp, Log, TEXT("OnClickedOption"));
}

void UTPSPauseMenuWidget::OnClickedQuit()
{
	UE_LOG(LogTemp, Log, TEXT("OnClickedQuit"));
	
}

void UTPSPauseMenuWidget::OnClickedContinue()
{
	UE_LOG(LogTemp, Log, TEXT("OnClickedContinue"));

	UCommonUIExtensions::PopContentFromLayer(this);
	SetMouseCursor(false);
}

void UTPSPauseMenuWidget::SetMouseCursor(const bool bShowMouseCursor)
{
	APlayerController* PC = TPSHelper::GetPlayerController(GetWorld());

	if (PC == nullptr)
	{
		return;
	}
	PC->SetShowMouseCursor(bShowMouseCursor);

	if (bShowMouseCursor == false)
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		PC->SetInputMode(FInputModeUIOnly());
	}
}
