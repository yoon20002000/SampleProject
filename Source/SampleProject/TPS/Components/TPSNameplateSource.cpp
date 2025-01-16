// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSNameplateSource.h"

#include "TPSHelper.h"
#include "Character/TPSAIController.h"
#include "Character/TPSCharacter.h"
#include "Character/TPSPlayerController.h"
#include "Components/TPSIndicatorManagerComponent.h"


UTPSNameplateSource::UTPSNameplateSource()
{
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UTPSNameplateSource::BeginPlay()
{
	Super::BeginPlay();

	if (ATPSCharacter* OwnerActor = Cast<ATPSCharacter>(GetOwner()))
	{
		if (ATPSAIController* AIController = Cast<ATPSAIController>(GetOwner()))
		{
			if (ATPSPlayerController* PlayerCharacter = Cast<ATPSPlayerController>(TPSHelper::GetPlayerController()))
			{
				if (UTPSIndicatorManagerComponent* IndicatorMG = PlayerCharacter->GetComponentByClass<UTPSIndicatorManagerComponent>())
				{
					//IndicatorMG->AddIndicator();
				}
			}
		}
	}
}

void UTPSNameplateSource::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (ATPSCharacter* OwnerActor = Cast<ATPSCharacter>(GetOwner()))
	{
		if (ATPSAIController* AIController = Cast<ATPSAIController>(GetOwner()))
		{
			if (ATPSPlayerController* PlayerCharacter = Cast<ATPSPlayerController>(TPSHelper::GetPlayerController()))
			{
				if (UTPSIndicatorManagerComponent* IndicatorMG = PlayerCharacter->GetComponentByClass<UTPSIndicatorManagerComponent>())
				{
					//IndicatorMG->UnregistIndicator(OwnerActor);
				}
			}
		}
	}
}
