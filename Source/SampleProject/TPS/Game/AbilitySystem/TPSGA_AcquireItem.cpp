// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSGA_AcquireItem.h"

#include "Character/TPSPlayer.h"
#include "Components/TPSInventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UTPSGA_AcquireItem::UTPSGA_AcquireItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTPSGA_AcquireItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// item을 캐릭터의 inventory에 추가 해야 됨
	// actorinfo를 통해 player class에 접근하고
	// actorinfo에서 inventory component를 받아와서 add 호출해서 추가해 줄 것
	// 획득 애니메이션 넣을 만한 것 있으면 넣어 줄 것
	// 파라미터에 있는 데이터에서 fitem 뽑아와서 적용 시킬 수 있어야 됨.

	UE_LOG(LogTemp, Log, TEXT("Acquire Item!!!"));
	
	ATPSPlayer* Player = Cast<ATPSPlayer>(CurrentActorInfo->AvatarActor.Get());

	if (Player == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UTPSInventoryComponent* InventoryComp = Player->GetComponentByClass<UTPSInventoryComponent>();

	if (InventoryComp == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	int32 SlotCount = 1;
	for (auto InventorySlot : InventoryComp->GetInventorySlots() )
	{
		UKismetSystemLibrary::PrintString(GetWorld(),FString::Format(TEXT("Inven Slot {0} : Item Name {1}, Quantity{2}"),
		                                                             {
			                                                             FStringFormatArg(SlotCount),
			                                                             FStringFormatArg(
				                                                             InventorySlot.ItemName.ToString()),
			                                                             FStringFormatArg(InventorySlot.ItemQuantity)
		                                                             }));
		++SlotCount;
	}
	
	
	if (CommitAbility(Handle,ActorInfo, ActivationInfo) == true)
	{
		InventoryComp->InteractionWithCurHitItem();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);	
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPSGA_AcquireItem::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
