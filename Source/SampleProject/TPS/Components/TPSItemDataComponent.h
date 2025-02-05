#pragma once

#include "TPSInventoryComponent.h"
#include "Components/ActorComponent.h"
#include "Game/TPSInteractionInterface.h"
#include "TPSItemDataComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSItemDataComponent : public UActorComponent,  public ITPSInteractionInterface
{
	GENERATED_BODY()

public:	
	UTPSItemDataComponent();
	
	/// TPSInteractionInterface Start
	virtual FItem* GetItemData() override;
	virtual void LookAtInteractionActor() override;
	virtual void Interaction() override;
	/// TPSInteractionInterface End
	/// 
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	FDataTableRowHandle ItemDataHandle;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	int32 Quantity;
};
