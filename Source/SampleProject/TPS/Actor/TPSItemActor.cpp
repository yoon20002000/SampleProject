#include "Actor/TPSItemActor.h"

#include "Components/TPSItemDataComponent.h"

// Sets default values
ATPSItemActor::ATPSItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ItemDataComponent = CreateDefaultSubobject<UTPSItemDataComponent>(TEXT("Item Data Comp"));
}

FItem ATPSItemActor::GetItemData()
{
	return ItemDataComponent->GetItemData();
}

void ATPSItemActor::LookAtInteractionActor()
{
	
}

void ATPSItemActor::Interaction()
{
	UE_LOG(LogTemp, Log, TEXT("Interaction can be able %s"), *GetItemData().Name.ToString());
}

// Called when the game starts or when spawned
void ATPSItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}
