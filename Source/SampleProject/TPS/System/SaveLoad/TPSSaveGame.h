#pragma once

#include "GameFramework/SaveGame.h"
#include "TPSSaveGame.generated.h"

struct FInventorySlot;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	void SetInventoryData(const TArray<FInventorySlot>& InventoryData);
	const TArray<FInventorySlot>& GetInventoryData() const;
private:
	UPROPERTY(meta=(AllowPrivateAccess=true))
	TArray<FInventorySlot> Inventory;
};
