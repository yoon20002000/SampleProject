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
	UE_LOG(LogTemp, Log, TEXT("Interaction can be able %s"), *GetItemData()->Name.ToString());
	GetOwner()->Destroy(true);
}

// Called when the game starts
void UTPSItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
