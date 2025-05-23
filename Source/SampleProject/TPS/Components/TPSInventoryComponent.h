// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TPSInventoryComponent.generated.h"

class UGameplayEffect;
class UTPSAbilitySystemComponent;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UGameplayEffect> ApplyEffectClass;
};
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
public:
	FInventorySlot();
	FInventorySlot(const FName& InItemName, const int32 InItemQuantity);
	bool IsEmpty() const;
private:
	void SetEmpty();
	void SetSlot(const FName& InItemName, const int32 Quantity);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity;
	
	friend class UTPSInventoryComponent;
};

DECLARE_MULTICAST_DELEGATE(OnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSInventoryComponent();
	// ItemName : Item Unique ID, Quantity : Must below Max Stack Size
	void AddItemToInventory(const FName& ItemName,const int32 Quantity);
	// Add Quantity & Broadcast Update
	void AddItemQuantity(int32 Quantity, FInventorySlot* Slot);
	// Add Quantity At Destination Slot and Start Slot, Broadcast Update 
	void AddQuantityClampMaxStack(FInventorySlot& SourceInventorySlot, FInventorySlot& DestinationInventorySlot,
								  int32 MaxStack);
	void AddItemToSlot(const int32 SlotIndex, const FName& ItemName,const int32 Quantity);
	// Get Max Stack Size from Item Data Table
	int32 GetMaxStackSize(const FName& ItemName) const;
	// Index : Remove Item Slot Index, bRemoveAll : Remove All Items, bIsConsumed : Is Used true
	void RemoveItem(const int32 Index, const bool bRemoveAll = false, const bool bIsConsumed = false);
	// Spawn Drop Item
	// ItemName : Drop Item Unique Name, Quantity : Drop Quantity
	void DropItem(const FName& ItemName, const int32 Quantity);
	// Find Same Item & below Max Stack Size 
	FInventorySlot* FindSameItemAddableSlot(const FName& ItemName);
	// Consume Item
	void ConsumeItem(UTPSAbilitySystemComponent* InstigatorASC, const int32 InventorySlotIndex);
	
	const TArray<FInventorySlot>& GetInventorySlots();
	void TransferSlots(const int32 SourceIndex, UTPSInventoryComponent* SourceInventoryComp, const int32 DestinationIndex);

	int32 GetInventorySlotSize() const;
	const FInventorySlot* GetInventorySlotOrNullptr(const int32 SlotIndex) const;
	FItem* GetItemDataOrNullptr(const FName& ItemName) const;

	bool HaveEnoughItems(const FName& ItemName, const int32 Quantity) const;

	void SetMaxSlotSize(int32 MaxStackSize);
	int32 GetMaxStackSize() const;
	/// GameData
	void SaveInventory();
	void LoadInventory();
protected:
	virtual void BeginPlay() override;
	// AddNewSlot at Item & Broadcast Update
	void AddNewItemToInventory(const FName& ItemName,const int32 Quantity);
private:
	
	void SwapInventorySlots(FInventorySlot& A, FInventorySlot& B);

public:
	OnInventoryUpdated OnInventoryUpdatedDelegate;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	float DropForwardDistance= 100;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	int32 InventorySlotMaxSize = 5;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = true))
	TArray<FInventorySlot> Inventory;
};
