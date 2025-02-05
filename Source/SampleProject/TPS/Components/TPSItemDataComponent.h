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
	FItem GetItemData() const;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	FDataTableRowHandle ItemDataHandle;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	int32 Quantity;
};
