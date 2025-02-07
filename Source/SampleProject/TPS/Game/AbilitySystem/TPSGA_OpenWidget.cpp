#include "Game/AbilitySystem/TPSGA_OpenWidget.h"

#include "TPSSystemManager.h"
#include "UI/TPSUIManager.h"

void UTPSGA_OpenWidget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UTPSSystemManager::Get()->GetUIManager()->LoadUI(OpenWidgetName);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UTPSGA_OpenWidget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	
}
