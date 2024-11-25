// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Action/Action.h"

#include "ActionComponent.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

class UWorld;

UActionComponent* UAction::GetOwningComponent() const
{
	return ActionComp;
}

FGameplayTag UAction::GetActivationTag() const
{
	return ActionTag;
}

bool UAction::IsAutoStart() const
{
	return bAutoStart;
}

void UAction::Initialize(UActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool UAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning() == true)
	{
		return false;
	}

	UActionComponent* AC = GetOwningComponent();

	if (AC->ActiveGameplayTags.HasAny(BlockedTags) == true)
	{
		return false;
	}
	return true;
}

void UAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOGFMT(LogTemp, Log, "Started: {ActionName}", GetName());

	UActionComponent* AC = GetOwningComponent();
	AC->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (AC->GetOwnerRole() == ROLE_Authority)
	{
		TimeStared = GetWorld()->TimeSeconds;
	}
	AC->OnActionStarted.Broadcast(AC, this);
}


void UAction::StopAction_Implementation(AActor* Instigator)
{
	UActionComponent* AC = GetOwningComponent();
	AC->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	AC ->OnActionStopped.Broadcast(AC, this);
}

UWorld* UAction::GetWorld() const
{
	if (AActor* Actor = Cast<AActor>(GetOuter()))
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

bool UAction::IsSupportedForNetworking() const
{
	return UObject::IsSupportedForNetworking();
}

void UAction::OnRep_RepData()
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

void UAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAction, RepData);
	DOREPLIFETIME(UAction, TimeStared);
	DOREPLIFETIME(UAction, ActionComp);
}
