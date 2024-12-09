// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Action/TPSActionComponent.h"

#include "TPSAction.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UTPSActionComponent::UTPSActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	bReplicateUsingRegisteredSubObjectList = true;
}


// Called when the game starts
void UTPSActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UTPSAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void UTPSActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<UTPSAction*> ActionCopy = Actions;
	
	for (UTPSAction* Action : ActionCopy)
	{
		if (Action->IsRunning() ==true)
		{
			Action->StopAction(GetOwner());
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UTPSActionComponent::ServerStopAction_Implementation(AActor* Instigator, FGameplayTag ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void UTPSActionComponent::ServerStartAction_Implementation(AActor* Instigator, FGameplayTag ActionName)
{
	StopActionByName(Instigator, ActionName);
}


// Called every frame
void UTPSActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTPSActionComponent::AddAction(AActor* Instigator, TSubclassOf<UTPSAction> ActionClass)
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

	UTPSAction* NewAction = NewObject<UTPSAction>(GetOwner(), ActionClass);
	check(NewAction);

	NewAction->Initialize(this);

	Actions.Add(NewAction);

	AddReplicatedSubObject(NewAction);

	if (NewAction->IsAutoStart() == true && ensure(NewAction->CanStart(Instigator)))
	{
		NewAction->StartAction(Instigator);
	}
}

void UTPSActionComponent::RemoveAction(UTPSAction* ActionToRemove)
{
	if (ensure(ActionToRemove && ActionToRemove->IsRunning() == false) == false)
	{
		return;
	}

	RemoveReplicatedSubObject(ActionToRemove);

	Actions.Remove(ActionToRemove);
}

UTPSAction* UTPSActionComponent::GetAction(const TSubclassOf<UTPSAction> ActionClass) const
{
	for (UTPSAction* Action : Actions)
	{
		if (Action->IsA(ActionClass) == true)
		{
			return Action;
		}
	}
	return nullptr;
}

bool UTPSActionComponent::StartActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for (UTPSAction* Action : Actions)
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

bool UTPSActionComponent::StopActionByName(AActor* Instigator, FGameplayTag ActionName)
{
	for (UTPSAction* Action : Actions)
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

void UTPSActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTPSActionComponent, Actions);
}
