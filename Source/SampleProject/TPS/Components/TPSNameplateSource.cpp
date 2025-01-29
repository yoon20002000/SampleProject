// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSNameplateSource.h"

#include "TPSHelper.h"
#include "TPSSystemManager.h"
#include "Character/TPSCharacter.h"
#include "Character/TPSPlayerController.h"
#include "System/TPSNameplateManager.h"


class UTPSNameplateManager;

UTPSNameplateSource::UTPSNameplateSource(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


// Called when the game starts
void UTPSNameplateSource::BeginPlay()
{
	Super::BeginPlay();

	if (ATPSCharacter* TargetCharacter = Cast<ATPSCharacter>(GetOwner()))
	{
		if (UTPSNameplateManager* NG = UTPSSystemManager::Get()->GetNameplateManager())
		{
			NG->RegistNameplate(TargetCharacter, NameplateWidgetClass);
		}
	}
}

void UTPSNameplateSource::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (UTPSNameplateManager* NG = UTPSSystemManager::Get()->GetNameplateManager())
	{
		NG->UnregistNameplate(Cast<APawn>(GetOwner()));
	}
}
