// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIKillCounter.h"

#include "CommonNumericTextBlock.h"
#include "Character/TPSPlayer.h"
#include "Character/TPSPlayerState.h"

void UUIKillCounter::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateKillCount(0);
}

void UUIKillCounter::InitKillCount(ATPSPlayer* Player)
{
	if (ATPSPlayerState* PS = Cast<ATPSPlayerState>(Player->GetPlayerState()))
	{
		PS->OnKillCountChanged.AddDynamic(this, &UUIKillCounter::OnUpdateKillCount);
	}
}

void UUIKillCounter::UninitKillCount(ATPSPlayer* Player)
{
	if (ATPSPlayerState* PS = Cast<ATPSPlayerState>(Player->GetPlayerState()))
	{
		PS->OnKillCountChanged.RemoveDynamic(this, &UUIKillCounter::OnUpdateKillCount);
	}
}

void UUIKillCounter::OnUpdateKillCount(ATPSPlayerState* PlayerState, uint32 KillCount, uint8 Delta)
{
	UpdateKillCount(KillCount);
}

void UUIKillCounter::UpdateKillCount(int32 InValue)
{
	if (KillCountText == nullptr)
	{
		return;
	}

	KillCountText->SetCurrentValue(InValue);
}
