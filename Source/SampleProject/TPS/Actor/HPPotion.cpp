// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/HPPotion.h"

#include "Character/TPSPlayer.h"

AHPPotion::AHPPotion()
{
}

void AHPPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Interact !! "));
	/*
	 * Temp Code
	 */
	if (ATPSPlayer* TP = Cast<ATPSPlayer>(InstigatorPawn))
	{
		TP->AddHP(RecoverPoint);
	}
}
