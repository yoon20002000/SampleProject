// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Action/ActionComponent.h"

#include "Action.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	bReplicateUsingRegisteredSubObjectList = true;
}


// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UAction*> ActionCopy = Actions;
	
	for (UAction* Action : Actions)
	{
		if (Action->IsRunning() ==true)
		{
			Action->StopAction(GetOwner());
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UActionComponent::ServerStopAction_Implementation(AActor* Instigator, FGameplayTag ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UActionComponent::ServerStartAction_Implementation(AActor* Instigator, FGameplayTag ActionName)
{
	StopActionByName(Instigator, ActionName);
}


// Called every frame
void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionComponent::AddAction(AActor* Instigator, TSubclassOf<UAction> ActionClass)
{
	if (ensure(ActionClass) == false)
	{
		return;
	}

	if (GetOwner()->HasAuthority() == false)
	{
		UE_LOGFMT(LogTemp, Warning, "Client attempting to add action. class : {class}", GetNameSafe(ActionClass));
		return;
	}

	UAction* NewAction = NewObject<UAction>(GetOwner(), ActionClass);
	check(NewAction);

	NewAction->Initialize(this);

	Actions.Add(NewAction);

	AddReplicatedSubObject(NewAction);

	if (NewAction->IsAutoStart() == true && ensure(NewAction->CanStart(Instigator)))
	{
		NewAction->StartAction(Instigator);
	}
}

void UActionComponent::RemoveAction(UAction* ActionToRemove)
{
	if (ensure(ActionToRemove && ActionToRemove->IsRunning() == false) == false)
	{
		return;
	}

	RemoveReplicatedSubObject(ActionToRemove);

	Actions.Remove(ActionToRemove);
}

UAction* UActionComponent::GetAction(const TSubclassOf<UAction> ActionClass) const
{
	for (UAction* Action : Actions)
	{
		if (Action->IsA(ActionClass) == true)
		{
			return Action;
		}
	}
	return nullptr;
}

bool UActionComponent::StartActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for (UAction* Action : Actions)
	{
		if (Action->GetActivationTag() == ActionName)
		{
			if (Action->CanStart(Instigator) == false)
			{
				FString FailedMsg = FString::Printf(TEXT("Faield to run : %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			if (GetOwner()->HasAuthority() == false)
			{
				ServerStartAction(Instigator, ActionName);
			}
			
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UActionComponent::StopActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for (UAction* Action : Actions)
	{
		if (Action->GetActivationTag() == ActionName)
		{

			if (GetOwner()->HasAuthority() == false)
			{
				ServerStartAction(Instigator, ActionName);
			}
			
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}

void UActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActionComponent, Actions);
}
