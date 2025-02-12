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
	TObjectPtr<UTexture2D> Thumbnail;
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

DECLARE_MULTICAST_DELEGATE(OnInventoryUpdated);

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
	FInventorySlot* FindSameItemAddableSlot(const FName& ItemName);
	
	const TArray<FInventorySlot>& GetInventorySlots();
	void InteractionWithCurHitItem();
	void TransferSlots(const int32 SourceIndex, UTPSInventoryComponent* SourceInventoryComp, const int32 DestinationIndex);

	int32 GetInventorySlotSize() const;
protected:
	virtual void BeginPlay() override;
	void AddNewItemToInventory(const FName& ItemName,const int32 Quantity);
private:
	void TraceItem();
	AActor* GetFirstHitItemInteraction(const TArray<FHitResult>& Hits) const;
	void SwapInventorySlots(FInventorySlot& A, FInventorySlot& B);

public:
	OnInventoryUpdated OnInventoryUpdatedDelegate;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	int32 InventorySlotMaxSize = 5;
	UPROPERTY()
	TArray<FInventorySlot> Inventory;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	float SweepDistance = 100;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	float SweepSphereRadius = 10;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurHitActor = nullptr;
};
