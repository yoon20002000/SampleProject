// Fill out your copyright notice in the Description page of Project Settings.


#include "System/TPSActorPoolingSubsystem.h"

#include "TPSActorPoolingInterface.h"
#include "Logging/StructuredLog.h"

static TAutoConsoleVariable CVarActorPoolingEnabled(
	TEXT("game.ActorPooling"),
	true,
	TEXT("Enable actor pooling for selected objects."),
	ECVF_Default);

AActor* UTPSActorPoolingSubsystem::SpawnActorPooled(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass,
	const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod SpawnHandling)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = SpawnHandling;

	return AcquireFromPool(WorldContextObject, ActorClass, SpawnTransform, Params);
}

bool UTPSActorPoolingSubsystem::ReleaseToPool(AActor* Actor)
{

	if (IsPoolingEnabled(Actor) == true)
	{
		UTPSActorPoolingSubsystem * PoolingSubsystem = Actor->GetWorld()->GetSubsystem<UTPSActorPoolingSubsystem>();
		return PoolingSubsystem->ReleaseToPool_Internal(Actor);
	}

	Actor->Destroy();
	return false;
}

AActor* UTPSActorPoolingSubsystem::AcquireFromPool(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass,
	const FTransform& SpawnTransform, FActorSpawnParameters SpawnParams)
{
	if (IsPoolingEnabled(WorldContextObject))
	{
		UTPSActorPoolingSubsystem* PoolingSubsystem = WorldContextObject->GetWorld()->GetSubsystem<UTPSActorPoolingSubsystem>();
		return PoolingSubsystem->AcquireFromPool_Internal(ActorClass, SpawnTransform,SpawnParams);
	}

	return WorldContextObject->GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);
}

bool UTPSActorPoolingSubsystem::IsPoolingEnabled(const UObject* WorldContextObject)
{
	return CVarActorPoolingEnabled.GetValueOnAnyThread() && WorldContextObject->GetWorld()->IsNetMode(NM_Standalone);
}

void UTPSActorPoolingSubsystem::PrimeActorPool(TSubclassOf<AActor> ActorClass, int32 Amount)
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

AActor* UTPSActorPoolingSubsystem::AcquireFromPool_Internal(TSubclassOf<AActor> ActorClass,
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

	ITPSActorPoolingInterface::Execute_PoolBeginPlay(AcquiredActor);
	return AcquiredActor;
}

bool UTPSActorPoolingSubsystem::ReleaseToPool_Internal(AActor* Actor)
{
	check(IsValid(Actor));

	Actor->SetActorEnableCollision(false);
	Actor->SetActorHiddenInGame(true);

	Actor->RouteEndPlay(EEndPlayReason::Destroyed);

	ITPSActorPoolingInterface::Execute_PoolEndPlay(Actor);

	FActorPool* ActorPool = &AvailableActorPool.FindOrAdd(Actor->GetClass());
	ActorPool->FreeActors.Add(Actor);
	
	return true;
}
