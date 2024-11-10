// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGameUIManagerSubsystem.h"

#include "Engine/GameInstance.h"
#include "TPSGameUIPolicy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TPSGameUIManagerSubsystem)

class FSubsystemCollectionBase;
class UClass;

void UTPSGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!CurrentPolicy && !DefaultUIPolicyClass.IsNull())
	{
		TSubclassOf<UTPSGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();
		if(PolicyClass == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("PolicyClass is nullptr"));
		}
		UTPSGameUIPolicy* GameUIPolicy = NewObject<UTPSGameUIPolicy>(this,PolicyClass);
		if(GameUIPolicy == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("GameUIPolicy is nullptr"));
		}
		SwitchToPolicy(GameUIPolicy);
	}
}

void UTPSGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	SwitchToPolicy(nullptr);
}

bool UTPSGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		// Only create an instance if there is no override implementation defined elsewhere
		return ChildClasses.Num() == 0;
	}

	return false;
}

void UTPSGameUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UTPSGameUIManagerSubsystem::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UTPSGameUIManagerSubsystem::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}

void UTPSGameUIManagerSubsystem::SwitchToPolicy(UTPSGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

