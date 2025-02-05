#include "Components/TPSItemDataComponent.h"

UTPSItemDataComponent::UTPSItemDataComponent() : Quantity(1)
{
	PrimaryComponentTick.bCanEverTick = false;
}
FItem* UTPSItemDataComponent::GetItemData()
{
	return ItemDataHandle.GetRow<FItem>(TEXT("Item Data"));
}

void UTPSItemDataComponent::LookAtInteractionActor()
{
	
}

void UTPSItemDataComponent::Interaction()
{
	GetOwner()->Destroy(true);
}

// Called when the game starts
void UTPSItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
