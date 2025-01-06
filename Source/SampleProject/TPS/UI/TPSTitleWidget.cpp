// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSTitleWidget.h"

#include "TPSHelper.h"
#include "TPSSystemManager.h"
#include "TPSUIManager.h"
#include "Components/Button.h"
#include "Game/TPSGameStateManager.h"
#include "Kismet/GameplayStatics.h"

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
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		UGameplayStatics::OpenLevel(TPSHelper::GetWorld(),FName(TEXT("MainGame")));
	}, 0.5f,false);
}

void UTPSTitleWidget::OnClickedQuitButton()
{
	UTPSSystemManager::Get()->QuitGame();
}
