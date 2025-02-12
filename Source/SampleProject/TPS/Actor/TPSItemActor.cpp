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

UTPSItemDataComponent* ATPSItemActor::GetItemDataComponent()
{
	return ItemDataComponent;
}

void ATPSItemActor::SetItemData(const FName& ItemName, const int32 Quantity)
{
	ItemDataComponent->SetItemData(ItemName, Quantity);
}
