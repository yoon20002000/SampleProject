// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "Character/TPSCharacter.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"
#include "Game/AbilitySystem/TPSGA_Attack.h"


UBTTask_Attack::UBTTask_Attack() : Character(nullptr)
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);;

	Character = Cast<ATPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character.IsValid() == false)
	{
		return EBTNodeResult::Failed;
	}

	UTPSAbilitySystemComponent* ASC = Character->GetTPSAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(GA_Attack);
	if (AbilitySpec != nullptr)
	{
		FGameplayAbilitySpecHandle AbilityHandle = AbilitySpec->Handle;

		if (ASC->TryActivateAbility(AbilityHandle) == true)
		{
			Character->OnEndDelegate.AddLambda(
				[this, &OwnerComp]()
				{
					FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);	
				});
			return EBTNodeResult::InProgress;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Failed : TryActivate Ability Failed!!!"));
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Failed : Ability Spec is nullptr !!!!"));
	}
	
	return EBTNodeResult::Failed;
}
