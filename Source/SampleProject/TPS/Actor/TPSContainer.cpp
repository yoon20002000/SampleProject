#include "Actor/TPSContainer.h"

#include "TPSSystemManager.h"
#include "Components/TPSInventoryComponent.h"
#include "UI/TPSUIManager.h"
#include "UI/Inventory/TPSContainerInventoryWidget.h"

ATPSContainer::ATPSContainer()
{
	InventoryComp = CreateDefaultSubobject<UTPSInventoryComponent>(TEXT("Inventory Comp"));
}

void ATPSContainer::Interaction(AActor* InteractionActor)
{
	Super::Interaction(InteractionActor);
	
	if (UIWidgetName != NAME_None)
	{
		if (UTPSContainerInventoryWidget* InstanceWidget =
			UTPSSystemManager::Get()->GetUIManager()->LoadUI<UTPSContainerInventoryWidget>(UIWidgetName))
		{
			InstanceWidget->Init(InventoryComp);
		}
	}
}

UTPSInventoryComponent* ATPSContainer::GetInventoryComponent() const
{
	return InventoryComp;
}
