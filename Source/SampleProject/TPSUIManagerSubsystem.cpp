// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSUIManagerSubsystem.h"

#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "GameFramework/HUD.h"

UTPSUIManagerSubsystem::UTPSUIManagerSubsystem()
{
}

void UTPSUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TickHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UTPSUIManagerSubsystem::Tick), 0.0f);
}

void UTPSUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
}

bool UTPSUIManagerSubsystem::Tick(float DeltaTime)
{
	SyncRootLayoutVisibilityToShowHUD();
	return true;
}

void UTPSUIManagerSubsystem::SyncRootLayoutVisibilityToShowHUD()
{
	if (const UYGameUIPolicy* Policy = GetCurrentUIPolicy())
	{
		for (const ULocalPlayer* LocalPlayer : GetGameInstance()->GetLocalPlayers())
		{
			bool bShouldShowUI = true;

			if (const APlayerController* PC = LocalPlayer->GetPlayerController(GetWorld()))
			{
				const AHUD* HUD = PC->GetHUD();

				if (HUD != nullptr && HUD->bShowHUD == false)
				{
					bShouldShowUI = false;
				}
			}

			if (UYPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UYCommonLocalPlayer>(LocalPlayer)))
			{
				const ESlateVisibility DesiredVisibility = bShouldShowUI == true
					                                           ? ESlateVisibility::SelfHitTestInvisible
					                                           : ESlateVisibility::Collapsed;
				if (DesiredVisibility != RootLayout->GetVisibility())
				{
					RootLayout->SetVisibility(DesiredVisibility);
				}
			}
		}
	}
}
