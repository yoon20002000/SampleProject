// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryGameLayout.h"

#include "CommonLocalPlayer.h"
#include "Kismet/GameplayStatics.h"

UYPrimaryGameLayout* UYPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	APlayerController* PlayerController = GameInstance->GetPrimaryPlayerController(false);
	return GetPrimaryGameLayout(PlayerController);
}

UYPrimaryGameLayout* UYPrimaryGameLayout::GetPrimaryGameLayout(APlayerController* PlayerController)
{
	return PlayerController != nullptr
		       ? GetPrimaryGameLayout(Cast<UYCommonLocalPlayer>(PlayerController->Player))
		       : nullptr;
}

UYPrimaryGameLayout* UYPrimaryGameLayout::GetPrimaryGameLayout(ULocalPlayer* LocalPlayer)
{
	if(LocalPlayer != nullptr)
	{
		const UYCommonLocalPlayer* CommonLocalPlayer = CastChecked<UYCommonLocalPlayer>(LocalPlayer);
		if(const UGameInstance* GameInstance = CommonLocalPlayer->GetGameInstance())
		{
			
		}
	}
	return nullptr;
}
