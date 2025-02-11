#include "Actor/TPSItemActor.h"

#include "Components/TPSItemDataComponent.h"

ATPSItemActor::ATPSItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	ItemDataComponent = CreateDefaultSubobject<UTPSItemDataComponent>(TEXT("Item Data Comp"));
}

void ATPSItemActor::Interaction(AActor* InteractionActor)
{
	if (UTPSInventoryComponent* InventoryComponent = InteractionActor->GetComponentByClass<UTPSInventoryComponent>())
	{
		ItemDataComponent->InteractionItem(InventoryComponent);
	}
}
