// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSTitleWidget.h"

#include "Components/Button.h"
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
}

void UTPSTitleWidget::OnClickedQuitButton()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->ConsoleCommand("quit");
	}
}
