#include "UI/Inventory/TPSPlayerMenuWidget.h"

#include "TPSHelper.h"
#include "TPSInventoryGridWidget.h"
#include "Character/TPSPlayer.h"
#include "Components/TPSInventoryComponent.h"

void UTPSPlayerMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetInputUIMode(true);
	if (APlayerController* PC = TPSHelper::GetPlayerController(GetWorld()))
	{
		if (ATPSPlayer* Player = Cast<ATPSPlayer>(PC->GetPawn()))
		{
			if (UTPSInventoryComponent* InventoryComp = Player->GetInventoryComponent())
			{
				InventoryGrid->Init(InventoryComp);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("In InventoryComp is nullptr!!"));
			}
		}
	}
}

void UTPSPlayerMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
	SetInputGameMode();
}
