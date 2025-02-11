#pragma once

#include "Actor/TPSApplyGEActor.h"
#include "TPSHPPotion.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSHPPotion : public ATPSApplyGEActor
{
	GENERATED_BODY()

public:
	ATPSHPPotion();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HP Potion")
	int32 RecoverPoint;
};
