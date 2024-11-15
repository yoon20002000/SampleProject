// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSCommonUserWidget.h"

#include "TPSHelper.h"
#include "Character/TPSPlayer.h"

ATPSPlayer* UTPSCommonUserWidget::GetTPSPlayer() const
{
	if (APlayerController* PlayerController = TPSHelper::GetPlayerController())
	{
		return CastChecked<ATPSPlayer>(PlayerController->GetPawn());
	}
	return nullptr;
}
