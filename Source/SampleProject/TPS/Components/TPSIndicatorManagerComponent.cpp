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

