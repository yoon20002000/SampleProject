#include "UI/Inventory/TPSContainerInventoryWidget.h"

#include "TPSHelper.h"
#include "Character/TPSPlayer.h"
#include "UI/Inventory/TPSInventoryGridWidget.h"
#include "Components/TPSInventoryComponent.h"

void UTPSContainerInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ATPSPlayer* Player = Cast<ATPSPlayer>(TPSHelper::GetPlayerController()->GetPawn()))
	{
		PlayerInventoryComp = Player->GetInventoryComponent();

		PlayerInventoryGrid->Init(PlayerInventoryComp.Get());
	}
}

void UTPSContainerInventoryWidget::Init(UTPSInventoryComponent* InventoryComp)
{
	ContainerInventoryComp = InventoryComp;
}
