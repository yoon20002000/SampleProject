// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSNameplateSource.h"

#include "TPSHelper.h"
#include "TPSNameplateManager.h"
#include "Character/TPSAIController.h"
#include "Character/TPSPlayerController.h"


UTPSNameplateSource::UTPSNameplateSource()
{
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UTPSNameplateSource::BeginPlay()
{
	Super::BeginPlay();
	
	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(TPSHelper::GetPlayerController()))
	{
		if (UTPSNameplateManager* NG = PC->GetComponentByClass<UTPSNameplateManager>())
		{
			NG->RegistNameplate(Cast<APawn>(GetOwner()), NameplateWidgetClass);
		}
	}
}

void UTPSNameplateSource::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(TPSHelper::GetPlayerController()))
	{
		if (UTPSNameplateManager* NG = PC->GetComponentByClass<UTPSNameplateManager>())
		{
			NG->UnregistNameplate(Cast<APawn>(GetOwner()));
		}
	}
}
