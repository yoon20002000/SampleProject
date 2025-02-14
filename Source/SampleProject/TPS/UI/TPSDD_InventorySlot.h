#pragma once

#include "UI/TPSDragDropOperation.h"
#include "TPSDD_InventorySlot.generated.h"

class UTPSInventoryComponent;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnDD_Cancelled)
UCLASS()
class SAMPLEPROJECT_API UTPSDD_InventorySlot : public UTPSDragDropOperation
{
	GENERATED_BODY()
public:
	void Init(UTPSInventoryComponent* InInventoryComponent, const int32 InContentIndex);
	void AddOnCancelledCallback(TFunction<void()> InCallback);
	int32 GetContentIndex() const;
	UTPSInventoryComponent* GetInventoryComponent() const;
	virtual void DragCancelled_Implementation(const FPointerEvent& PointerEvent) override;
protected:
	virtual void BeginDestroy() override;
private:
	FOnDD_Cancelled OnCancelled;

	TWeakObjectPtr<UTPSInventoryComponent> InventoryComp;
	int32 ContentIndex;	
};
