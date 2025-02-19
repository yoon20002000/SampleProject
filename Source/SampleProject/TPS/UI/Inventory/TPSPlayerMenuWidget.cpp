#include "UI/Inventory/TPSPlayerMenuWidget.h"

#include "TPSHelper.h"
#include "TPSInventoryGridWidget.h"
#include "Character/TPSPlayer.h"
#include "Components/TPSInventoryComponent.h"
#include "UI/Status/TPSStatusWidget.h"

void UTPSPlayerMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetInputUIMode(true);
	
	if (ATPSPlayer* Player = TPSHelper::GetPlayerOrNullptr())
	{
		if (UTPSInventoryComponent* InventoryComp = Player->GetInventoryComponent())
		{
			InventoryGrid->Init(InventoryComp);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("In InventoryComp is nullptr!!"));
		}

		StatusWidget->Init(*Player);
	}
}

void UTPSPlayerMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (ATPSPlayer* Player = TPSHelper::GetPlayerOrNullptr())
	{
		Player->SetActiveSceneCaptureComp(false);
	}
	
	SetInputGameMode();
}
