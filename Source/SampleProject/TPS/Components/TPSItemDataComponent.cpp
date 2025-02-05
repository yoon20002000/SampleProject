#include "Components/TPSItemDataComponent.h"

UTPSItemDataComponent::UTPSItemDataComponent() : Quantity(1)
{
	PrimaryComponentTick.bCanEverTick = false;
}

FItem UTPSItemDataComponent::GetItemData() const
{
	return *ItemDataHandle.GetRow<FItem>(TEXT("Item Data"));
}


// Called when the game starts
void UTPSItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
