// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSIndicatorManagerComponent.h"

#include "UI/TPSIndicatorDescriptor.h"


// Sets default values for this component's properties
UTPSIndicatorManagerComponent::UTPSIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoRegister = true;
	bAutoActivate = true;
}

UTPSIndicatorManagerComponent* UTPSIndicatorManagerComponent::GetComponent(AController* Controller)
{
	if (Controller == nullptr)
	{
		return nullptr;
	}

	return Controller ->FindComponentByClass<UTPSIndicatorManagerComponent>();
}

void UTPSIndicatorManagerComponent::AddIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor == nullptr)
	{
		return ;
	}

	IndicatorDescriptor->SetIndicatorManagerComponent(this);
	OnIndicatorAdded.Broadcast(IndicatorDescriptor);
	Indicators.Add(IndicatorDescriptor);
}

void UTPSIndicatorManagerComponent::RemoveIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor == nullptr)
	{
		return;
	}

	if (IndicatorDescriptor->GetIndicatorManagerComponent() == this)
	{
		OnIndicatorRemoved.Broadcast(IndicatorDescriptor);
		Indicators.Remove(IndicatorDescriptor);
	}
}
const TArray<UTPSIndicatorDescriptor*>& UTPSIndicatorManagerComponent::GetIndicators() const
{
	return Indicators;
}

