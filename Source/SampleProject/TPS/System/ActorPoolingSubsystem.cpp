// Fill out your copyright notice in the Description page of Project Settings.


#include "System/ActorPoolingSubsystem.h"

#include "ActorPoolingInterface.h"
#include "Logging/StructuredLog.h"

static TAutoConsoleVariable CVarActorPoolingEnabled(
	TEXT("game.ActorPooling"),
	true,
	TEXT("Enable actor pooling for selected objects."),
	ECVF_Default);

AActor* UActorPoolingSubsystem::SpawnActorPooled(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass,
	const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod SpawnHandling)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = SpawnHandling;

	return AcquireFromPool(WorldContextObject, ActorClass, SpawnTransform, Params);
}

bool UActorPoolingSubsystem::ReleaseToPool(AActor* Actor)
{

	if (IsPoolingEnabled(Actor) == true)
	{
		UActorPoolingSubsystem * PoolingSubsystem = Actor->GetWorld()->GetSubsystem<UActorPoolingSubsystem>();
		return PoolingSubsystem->ReleaseToPool_Internal(Actor);
	}

	Actor->Destroy();
	return false;
}

AActor* UActorPoolingSubsystem::AcquireFromPool(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass,
	const FTransform& SpawnTransform, FActorSpawnParameters SpawnParams)
{
	if (IsPoolingEnabled(WorldContextObject))
	{
		UActorPoolingSubsystem* PoolingSubsystem = WorldContextObject->GetWorld()->GetSubsystem<UActorPoolingSubsystem>();
		return PoolingSubsystem->AcquireFromPool_Internal(ActorClass, SpawnTransform,SpawnParams);
	}

	return WorldContextObject->GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);
}

bool UActorPoolingSubsystem::IsPoolingEnabled(const UObject* WorldContextObject)
{
	return CVarActorPoolingEnabled.GetValueOnAnyThread() && WorldContextObject->GetWorld()->IsNetMode(NM_Standalone);
}

void UActorPoolingSubsystem::PrimeActorPool(TSubclassOf<AActor> ActorClass, int32 Amount)
{
	UE_LOGFMT(LogTemp, Log, "Priming Pool for {actorclass} ({amount})", GetNameSafe(ActorClass), Amount);

	for (int i = 0; i < Amount; ++i)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, FTransform::Identity, Params);

		ReleaseToPool(NewActor);
	}
}

AActor* UActorPoolingSubsystem::AcquireFromPool_Internal(TSubclassOf<AActor> ActorClass,
	const FTransform& SpawnTransform, FActorSpawnParameters SpawnParams)
{
	AActor* AcquiredActor = nullptr;

	FActorPool* ActorPool = &AvailableActorPool.FindOrAdd(ActorClass);

	if (ActorPool->FreeActors.IsValidIndex(0))
	{
		UE_LOGFMT(LogTemp, Log, "Acquired Actor for {actorclass} from pool", GetNameSafe(ActorClass));

		AcquiredActor = ActorPool->FreeActors[0];
		ActorPool->FreeActors.RemoveAt(0,1,EAllowShrinking::Yes);
	}

	if (AcquiredActor == nullptr)
	{
		UE_LOGFMT(LogTemp, Log, "Actor Pool empty, spawning new Actor for {actorclass}", GetNameSafe(ActorClass));

		return GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);
	}

	AcquiredActor->SetActorTransform(SpawnTransform);
	AcquiredActor->SetInstigator(SpawnParams.Instigator);
	AcquiredActor->SetOwner(SpawnParams.Owner);

	AcquiredActor->SetActorEnableCollision(true);
	AcquiredActor->SetActorHiddenInGame(false);

	AcquiredActor->DispatchBeginPlay();

	IActorPoolingInterface::Execute_PoolBeginPlay(AcquiredActor);
	return AcquiredActor;
}

bool UActorPoolingSubsystem::ReleaseToPool_Internal(AActor* Actor)
{
	check(IsValid(Actor));

	Actor->SetActorEnableCollision(false);
	Actor->SetActorHiddenInGame(true);

	Actor->RouteEndPlay(EEndPlayReason::Destroyed);

	IActorPoolingInterface::Execute_PoolEndPlay(Actor);

	FActorPool* ActorPool = &AvailableActorPool.FindOrAdd(Actor->GetClass());
	ActorPool->FreeActors.Add(Actor);
	
	return true;
}
