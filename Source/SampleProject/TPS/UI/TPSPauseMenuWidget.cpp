// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSPauseMenuWidget.h"

#include "Components/Button.h"

bool UTPSPauseMenuWidget::Initialize()
{
	bool bResult = Super::Initialize();

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
}

void UTPSPauseMenuWidget::OnClickedQuit()
{
}

void UTPSPauseMenuWidget::OnClickedContinue()
{
}
