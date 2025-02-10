#pragma once

#include "UI/TPSDragDropOperation.h"
#include "TPSDD_InventorySlot.generated.h"

class UTPSInventoryComponent;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSDD_InventorySlot : public UTPSDragDropOperation
{
	GENERATED_BODY()
public:
	void Init(UTPSInventoryComponent* InInventoryComponent, const int32 InContentIndex);
	int32 GetContentIndex() const;
	UTPSInventoryComponent* GetInventoryComponent() const;
private:
	TWeakObjectPtr<UTPSInventoryComponent> InventoryComp;
	int32 ContentIndex;
};
