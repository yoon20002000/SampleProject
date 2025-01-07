#include "Game/AbilitySystem/TPSGA_Interaction.h"


UTPSGA_Interaction::UTPSGA_Interaction(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UTPSGA_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 추후 interaction 하는데 필요한 cost 가 있는 ge를 가지고 있는 field object일 경우 체크 필요
	if (CommitAbility(Handle, ActorInfo, ActivationInfo) == true)
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,false);	
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPSGA_Interaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
