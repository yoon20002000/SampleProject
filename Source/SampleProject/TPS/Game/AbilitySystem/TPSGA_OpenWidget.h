#pragma once

#include "Game/TPSGameplayAbility.h"
#include "TPSGA_OpenWidget.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGA_OpenWidget : public UTPSGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
private:
	UPROPERTY(EditDefaultsOnly, meta=(allowPrivateAccess=true))
	FName OpenWidgetName;
};
