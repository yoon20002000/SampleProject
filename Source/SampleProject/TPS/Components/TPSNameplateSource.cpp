// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSNameplateSource.h"

#include "TPSHelper.h"
#include "TPSIndicatorManagerComponent.h"
#include "TPSNameplateManager.h"
#include "Character/TPSAIController.h"
#include "Character/TPSPlayerController.h"


UTPSNameplateSource::UTPSNameplateSource(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


// Called when the game starts
void UTPSNameplateSource::BeginPlay()
{
	Super::BeginPlay();
	
	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(TPSHelper::GetPlayerController()))
	{
		if (UTPSIndicatorManagerComponent* MG = PC->GetComponentByClass<UTPSIndicatorManagerComponent>())
		{
			UE_LOG(LogTemp, Log, TEXT("MG"));
		}
		
		if (UTPSNameplateManager* NG = PC->GetComponentByClass<UTPSNameplateManager>())
		{
			UE_LOG(LogTemp, Log, TEXT("NG"));
			APawn* IndicatorPawn = Cast<ATPSAIController>(GetOwner())->GetPawn();
			NG->RegistNameplate(IndicatorPawn, NameplateWidgetClass);
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
