#pragma once

#include "CoreMinimal.h"
#include "Character/TPSCharacter.h"
#include "Components/TPSThreatSenseComponent.h"
#include "TPSNonPlayer.generated.h"

class UTPSThreatSenseComponent;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API ATPSNonPlayer : public ATPSCharacter
{
	GENERATED_BODY()
public:
	ATPSNonPlayer();

	virtual void OnHealthChanged(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* InstigatorActor) override;
	virtual void Tick(float DeltaSeconds) override;
	FORCEINLINE float GetAttackRange() const
	{
		return AttackRange;
	}
private:
	UPROPERTY(EditAnywhere)
	float AttackRange;

	/// ThreatSystem Section
public:
	FORCEINLINE AActor* GetHighestTarget() const
	{
		return ThreatSenseComponent->GetHighestActor();
	}
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTPSThreatSenseComponent> ThreatSenseComponent;
	///
};
