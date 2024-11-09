// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUIManagerSubsystem.h"

#include "GameUIPolicy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIManagerSubsystem)

UYGameUIManagerSubsystem::UYGameUIManagerSubsystem()
{
}

void UYGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if(CurrentPolicy == nullptr && DefaultUIPolicyClass.IsNull() == false)
	{
		TSubclassOf<UYGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();
		if(PolicyClass == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("PolicyClass is nullptr"));
		}
		UYGameUIPolicy* GameUIPolicy = NewObject<UYGameUIPolicy>(this,PolicyClass);
		if(GameUIPolicy == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("GameUIPolicy is nullptr"));
		}
		SwitchToPolicy(GameUIPolicy);
	}
}

void UYGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	SwitchToPolicy(nullptr);
}

bool UYGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if(CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance() == false)
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		return ChildClasses.Num() == 0;
	}
	
	return false;
}

void UYGameUIManagerSubsystem::NotifyPlayerAdded(UYCommonLocalPlayer* LocalPlayer)
{
	if(ensure(LocalPlayer) && CurrentPolicy != nullptr)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UYGameUIManagerSubsystem::NotifyPlayerRemoved(UYCommonLocalPlayer* LocalPlayer)
{
	if(ensure(LocalPlayer) && CurrentPolicy != nullptr)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UYGameUIManagerSubsystem::NotifyPlayerDestroyed(UYCommonLocalPlayer* LocalPlayer)
{
	if(ensure(LocalPlayer) && CurrentPolicy != nullptr)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}

void UYGameUIManagerSubsystem::SwitchToPolicy(UYGameUIPolicy* InPolicy)
{
	if(CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}
