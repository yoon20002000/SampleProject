#include "UI/Inventory/TPSPlayerMenuWidget.h"

#include "TPSHelper.h"
#include "TPSInventoryGridWidget.h"
#include "Character/TPSPlayer.h"
#include "Components/TPSInventoryComponent.h"

void UTPSPlayerMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	APlayerController* PC = TPSHelper::GetPlayerController();
	if (ATPSPlayer* Player = Cast<ATPSPlayer>(PC->GetPawn()))
	{
		InventoryComp = Player->GetComponentByClass<UTPSInventoryComponent>();
	}

	if (InventoryComp != nullptr)
	{
		InventoryGrid->Init(InventoryComp.Get());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("In InventoryComp is nullptr!!"));
	}
}
