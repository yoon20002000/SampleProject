#pragma once

#include "TPSWorldData.generated.h"

class ATPSContainer;

// NormalActorData
USTRUCT(BlueprintType)
struct FWorldActorData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FWorldActorData() : Actor(nullptr), ActorTransform(FTransform::Identity)
	{
		
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> Actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;
};

// ItemComponent에 들어 있을 Item의 Data
USTRUCT(BlueprintType)
struct FItemComponentData
{
	GENERATED_BODY()
public:
	FItemComponentData() : InventorySlotIndex(-1), Item(), Quantity(-1)
	{
		
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySlotIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
};

// ContainerActor의 ItemComponent 설정용 Data
USTRUCT(BlueprintType)
struct FContainerActorData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FContainerActorData() : ContainerActor(nullptr), ActorTransform(FTransform::Identity), InventoryMaxStackSize(1),
	                        ContainedItems()
	{
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<ATPSContainer> ContainerActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventoryMaxStackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemComponentData> ContainedItems;
};
