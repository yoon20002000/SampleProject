// Fill out your copyright notice in the Description page of Project Settings.


#include "System/TickablesSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TickablesSubsystem)


static TAutoConsoleVariable CVarAggregateTicks(
	TEXT("game.AggregateTicks"),
	true,
	TEXT("Enable aggregate ticking for selected objects. Takes effect on next level load."),
	ECVF_Default);

void FTickablesTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
	const FGraphEventRef& CompletionGraphEvent)
{
	Target->ExecuteTick(TickGroup, DeltaTime, TickType, CurrentThread, CompletionGraphEvent);
}

void UTickablesSubsystem::RegisterComponent(FActorComponentTickFunction* TickFunction)
{
	if (CVarAggregateTicks.GetValueOnGameThread())
	{
		TickFunction->UnRegisterTickFunction();

		check(TickableComponents.Contains(TickFunction) == false);
		TickableComponents.Add(TickFunction);
	}
}

void UTickablesSubsystem::UnregisterComponent(FActorComponentTickFunction* TickFunction)
{
	if (CVarAggregateTicks.GetValueOnGameThread())
	{
		CleanupQueue.Add(TickFunction);
	}
}

void UTickablesSubsystem::ExecuteTick(ETickingGroup TickGroup, float DeltaTime, ELevelTick TickType,
	ENamedThreads::Type CurrentThread, const FGraphEventRef& CompletionGraphEvent)
{
	for (FActorComponentTickFunction* Func : TickableComponents)
	{
		Func->ExecuteTick(DeltaTime, TickType, CurrentThread, CompletionGraphEvent);
	}

	for (FActorComponentTickFunction* Func : CleanupQueue)
	{
		TickableComponents.Remove(Func);
	}
	CleanupQueue.Empty();
}

void UTickablesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (CVarAggregateTicks.GetValueOnGameThread())
	{
		ComponentsTick.bCanEverTick = true;
		ComponentsTick.Target = this;
		ComponentsTick.TickGroup = TG_PrePhysics;

		ComponentsTick.bRunOnAnyThread = false;

		ComponentsTick.RegisterTickFunction(GetWorld()->PersistentLevel);
	}
}
