// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Action/TPSAction.h"

#include "TPSActionComponent.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

class UWorld;

UTPSActionComponent* UTPSAction::GetOwningComponent() const
{
	return ActionComp;
}

FGameplayTag UTPSAction::GetActivationTag() const
{
	return ActionTag;
}

bool UTPSAction::IsAutoStart() const
{
	return bAutoStart;
}

void UTPSAction::Initialize(UTPSActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UTPSAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool UTPSAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning() == true)
	{
		return false;
	}

	UTPSActionComponent* AC = GetOwningComponent();

	if (AC->ActiveGameplayTags.HasAny(BlockedTags) == true)
	{
		return false;
	}
	return true;
}

void UTPSAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOGFMT(LogTemp, Log, "Started: {ActionName}", GetName());

	UTPSActionComponent* AC = GetOwningComponent();
	AC->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (AC->GetOwnerRole() == ROLE_Authority)
	{
		TimeStared = GetWorld()->TimeSeconds;
	}
	AC->OnActionStarted.Broadcast(AC, this);
}


void UTPSAction::StopAction_Implementation(AActor* Instigator)
{
UE_LOGFMT(LogTemp, Log, "Stop: {ActionName}", GetName());
	UTPSActionComponent* AC = GetOwningComponent();
	AC->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	AC->OnActionStopped.Broadcast(AC, this);
}

UWorld* UTPSAction::GetWorld() const
{
	if (AActor* Actor = Cast<AActor>(GetOuter()))
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

bool UTPSAction::IsSupportedForNetworking() const
{
	return UObject::IsSupportedForNetworking();
}

void UTPSAction::OnRep_RepData()
{
	if (IsRunning() == true)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void UTPSAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTPSAction, RepData);
	DOREPLIFETIME(UTPSAction, TimeStared);
	DOREPLIFETIME(UTPSAction, ActionComp);
}
