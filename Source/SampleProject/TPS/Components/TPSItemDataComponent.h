#pragma once

#include "TPSInventoryComponent.h"
#include "Components/ActorComponent.h"
#include "TPSItemDataComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSItemDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSItemDataComponent();
	FItem* GetItemData();
	void InteractionItem(UTPSInventoryComponent* InventoryComponent);
	// 런타임 Item Actor 생성 시 데이터 세팅용
	void SetItemData(const FName& ItemName, const int32 InQuantity);
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	FDataTableRowHandle ItemDataHandle;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	int32 Quantity;
};
