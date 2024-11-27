// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Action/ActionEffect.h"

#include "ActionComponent.h"
#include "GameFramework/GameStateBase.h"

UActionEffect::UActionEffect()
{
	bAutoStart = true;
}

float UActionEffect::GetTimeRemaining() const
{
	if (AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>())
	{
		float EndTime = TimeStared + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}
	
	return Duration;
}

void UActionEffect::StartAction_Implementation(AActor* Instigator)
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

void UActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	
	if (UActionComponent* AC = GetOwningComponent())
	{
		AC->RemoveAction(this);	
	}
}

void UActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
