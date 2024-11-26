// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "TickablesSubsystem.generated.h"

USTRUCT()
struct FTickablesTickFunction : public FTickFunction
{
	GENERATED_BODY()
	UPROPERTY()
	UTickablesSubsystem* Target = nullptr;
	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& CompletionGraphEvent) override;

	virtual FString DiagnosticMessage() override
	{
		return TEXT("FAggregateTickFunction");
	}
};

template<>
struct TStructOpsTypeTraits<FTickablesTickFunction> : TStructOpsTypeTraitsBase2<FTickablesTickFunction>
{
	enum
	{
		WithCopy = false
	};
};
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTickablesSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterComponent(FActorComponentTickFunction* TickFunction);
	void UnregisterComponent(FActorComponentTickFunction* TickFunction);
	void ExecuteTick(ETickingGroup TickGroup, float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
	                 const FGraphEventRef& CompletionGraphEvent);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	TArray<FActorComponentTickFunction*> TickableComponents;
	FTickablesTickFunction ComponentsTick;
	TArray<FActorComponentTickFunction*> CleanupQueue;
};
