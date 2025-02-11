// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSGameResultWidget.h"

#include "TPSHelper.h"
#include "Character/TPSPlayer.h"
#include "Components/Button.h"
#include "CommonTextBlock.h"
#include "TPSSystemManager.h"
#include "Game/TPSGameStateManager.h"

bool UTPSGameResultWidget::Initialize()
{
	bool bResult = Super::Initialize();

	if (GoToTitleButton != nullptr)
	{
		GoToTitleButton->OnClicked.AddDynamic(this, &UTPSGameResultWidget::OnClickedRetryButton);
	}
	
	return bResult;
}

void UTPSGameResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PC = TPSHelper::GetPlayerController(GetWorld());

	APawn* PlayerPawn = PC->GetPawn();
	bool bIsClear = false;
	if (ATPSPlayer* Player = Cast<ATPSPlayer>(PlayerPawn))
	{
		if (UTPSHealthComponent* AC = Player->GetHealthAttributeComp())
		{
			bIsClear = AC->IsAlive();	
		}
	}

	FText TextContent = FText::FromString(bIsClear == true ? TEXT("Clear!!") : TEXT("You Lose!!"));
	GameResultText->SetText(TextContent);
}

void UTPSGameResultWidget::OnClickedRetryButton()
{
	UTPSSystemManager::Get()->GetGameStateManager()->SetGameplayState(EGameplayState::GameReplay);
}
