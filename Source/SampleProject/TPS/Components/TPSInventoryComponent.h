// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TPSInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	FItem();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackSize;
};
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	UPROPERTY()
	FName ItemName;
	UPROPERTY()
	int32 ItemQuantity;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AddItemToInventory(const FName& ItemName,const int32 Quantity);
	int32 GetMaxStackSize(const FName& ItemName) const;
	void RemoveItem();
	FInventorySlot* FindAddSlot(const FName& ItemName);
	void CreateNewSlotAndAddToInventory(const FName& ItemName,const int32 Quantity);
	TArray<FInventorySlot> GetInventorySlots();
	void InteractionWithCurHitItem();
protected:
	virtual void BeginPlay() override;
private:
	void TraceItem();
	AActor* GetFirstHitItemInteraction(const TArray<FHitResult>& Hits) const;
	

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	int32 InventoryMaxSize = 5;
	UPROPERTY()
	TArray<FInventorySlot> Inventory;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	float SweepDistance = 100;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	float SweepSphereRadius = 10;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurHitActor = nullptr;
};
