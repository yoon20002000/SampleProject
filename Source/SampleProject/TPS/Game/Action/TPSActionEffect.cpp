// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Action/TPSActionEffect.h"

#include "TPSActionComponent.h"
#include "TPSHelper.h"
#include "GameFramework/GameStateBase.h"

UTPSActionEffect::UTPSActionEffect()
{
	bAutoStart = true;
}

float UTPSActionEffect::GetTimeRemaining() const
{
	if (AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>())
	{
		float EndTime = TimeStared + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	
	return Duration;
}

void UTPSActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction",Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect",Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate,Period,true);
	}
}

void UTPSActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	
	if (UTPSActionComponent* AC = GetOwningComponent())
	{
		AC->RemoveAction(this);	
	}
}

void UTPSActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	if (Instigator != nullptr)
	{
		TPSHelper::ApplyDamage(Instigator, GetOwningComponent()->GetOwner(), Damage);
	}
}
