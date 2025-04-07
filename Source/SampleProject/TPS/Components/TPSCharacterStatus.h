#pragma once

#include "Components/ActorComponent.h"
#include "TPSCharacterStatus.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpdated, int32, level);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SAMPLEPROJECT_API UTPSCharacterStatus : public UActorComponent
{
	GENERATED_BODY()

public:
	UTPSCharacterStatus();

	FORCEINLINE int32 GetLevel() const
	{
		return Level;
	}
	FORCEINLINE void SetLevel(int32 InLevel)
	{
		Level = InLevel;
		OnLevelUpdated.Broadcast(InLevel);
	}

protected:
	virtual void BeginPlay() override;

public:
	FOnLevelUpdated OnLevelUpdated;

private:
	UPROPERTY(EditAnywhere,meta=(allowprint = true))
	int32 Level;
};
