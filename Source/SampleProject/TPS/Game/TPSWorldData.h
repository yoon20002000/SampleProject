#pragma once

#include "TPSWorldData.generated.h"

class ATPSContainer;

USTRUCT(BlueprintType)
struct FWorldActorData : public FTableRowBase
{
	GENERATED_BODY()
	FWorldActorData() : Actor(nullptr), ActorTransform(FTransform::Identity)
	{
		
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> Actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;
};

USTRUCT(BlueprintType)
struct FItemComponentData
{
	GENERATED_BODY()
	FItemComponentData() : InventorySlotIndex(-1), Item(), Quantity(-1)
	{
		
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySlotIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
};

USTRUCT(BlueprintType)
struct FContainerActorData : public FTableRowBase
{
	GENERATED_BODY()
	FContainerActorData() : ContainerActor(nullptr), ActorTransform(FTransform::Identity), InventoryMaxStackSize(1),
	                        ContainedItems()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<ATPSContainer> ContainerActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventoryMaxStackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemComponentData> ContainedItems;
};
