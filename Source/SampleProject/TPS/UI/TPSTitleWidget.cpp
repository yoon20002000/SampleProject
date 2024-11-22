// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSTitleWidget.h"

#include "TPSSystemManager.h"
#include "TPSUIManager.h"
#include "Components/Button.h"
#include "Game/TPSGameStateManager.h"

void UTPSTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (PlayButton != nullptr)
	{
		PlayButton->OnClicked.AddDynamic(this, &UTPSTitleWidget::OnClickedPlayButton);
	}

	if (QuitButton != nullptr)
	{
		QuitButton->OnClicked.AddDynamic(this, &UTPSTitleWidget::OnClickedQuitButton);
	}
}

void UTPSTitleWidget::OnClickedPlayButton()
{
	UTPSSystemManager::Get()->GetUIManager()->RemoveUI(this);

	UTPSSystemManager::Get()->SetGameState(EGameplayState::MainGame);
}

void UTPSTitleWidget::OnClickedQuitButton()
{
	UTPSSystemManager::Get()->QuitGame();
}
