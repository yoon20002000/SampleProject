// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSNameplateSource.h"

#include "TPSHelper.h"
#include "TPSNameplateManager.h"
#include "Character/TPSCharacter.h"
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
		if (ATPSCharacter* TargetCharacter = Cast<ATPSCharacter>(GetOwner()))
		{
			UE_LOG(LogTemp, Log,TEXT("네임 플레이트 생성 시도"));
			{
				if (UTPSNameplateManager* NG = PC->GetComponentByClass<UTPSNameplateManager>())
				{
					UE_LOG(LogTemp, Log, TEXT("NG"));
					NG->RegistNameplate(TargetCharacter, NameplateWidgetClass);
				}
			}
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
