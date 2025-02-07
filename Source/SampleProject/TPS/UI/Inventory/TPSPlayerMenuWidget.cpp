#include "UI/Inventory/TPSPlayerMenuWidget.h"

#include "TPSHelper.h"
#include "TPSInventoryGridWidget.h"
#include "Character/TPSPlayer.h"
#include "Components/TPSInventoryComponent.h"

void UTPSPlayerMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (APlayerController* PC = TPSHelper::GetPlayerController(GetWorld()))
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);

		if (ATPSPlayer* Player = Cast<ATPSPlayer>(PC->GetPawn()))
		{
			InventoryComp = Player->GetComponentByClass<UTPSInventoryComponent>();
			if (InventoryComp != nullptr)
			{
				InventoryGrid->Init(InventoryComp.Get());
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
	if (APlayerController* PC = TPSHelper::GetPlayerController(GetWorld()))
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}
