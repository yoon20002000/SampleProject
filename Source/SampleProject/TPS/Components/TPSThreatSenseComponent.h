#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSThreatSenseComponent.generated.h"

class ATPSPlayer;
class ATPSCharacter;

UENUM(BlueprintType)
enum class EThreatLevel : uint8
{
	Calm  UMETA(DisplayName = "Calm"),
	Alert UMETA(DisplayName = "Alert"),
	Enraged UMETA(DisplayName = "Enraged"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSThreatSenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSThreatSenseComponent();

	void ReportThreat(ATPSPlayer* InSourceActor, float InAmount);

	void ThreatDecay(float InDeltaTime);

	EThreatLevel GetCurrentThreatLevel() const;

	AActor* GetHighestActor() const;
protected:
	virtual void BeginPlay() override;

	TMap<TWeakObjectPtr<ATPSCharacter>, float> ThreatMap;

	UPROPERTY(EditAnywhere, Category = Threat)
	float ThreatDecayAmount;

	UPROPERTY(EditAnywhere, Category = Threat)
	float ThreatThreshold;

	UPROPERTY(EditAnywhere, Category = Threat)
	float AlertThreshold;

	class ATPSAIController* AIController;
};
