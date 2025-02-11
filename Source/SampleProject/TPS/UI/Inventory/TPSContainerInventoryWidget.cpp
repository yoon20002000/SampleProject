#include "UI/Inventory/TPSContainerInventoryWidget.h"

#include "TPSHelper.h"
#include "Character/TPSPlayer.h"
#include "UI/Inventory/TPSInventoryGridWidget.h"
#include "Components/TPSInventoryComponent.h"

void UTPSContainerInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetInputUIMode(true);
	
	if (ATPSPlayer* Player = Cast<ATPSPlayer>(TPSHelper::GetPlayerController()->GetPawn()))
	{
		PlayerInventoryComp = Player->GetInventoryComponent();

		PlayerInventoryGrid->Init(PlayerInventoryComp.Get());
	}
}

void UTPSContainerInventoryWidget::NativeDestruct()
{
	SetInputGameMode();
}

void UTPSContainerInventoryWidget::Init(UTPSInventoryComponent* InventoryComp)
{
	ContainerInventoryComp = InventoryComp;
	ContainerInventoryGrid->Init(ContainerInventoryComp.Get());
}
