// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSIndicatorDescriptor.h"


UTPSIndicatorDescriptor::UTPSIndicatorDescriptor()
{
}

UObject* UTPSIndicatorDescriptor::GetDataObject() const
{
	return DataObject;
}

void UTPSIndicatorDescriptor::SetDataObject(UObject* InDataObject)
{
	DataObject = InDataObject;
}

USceneComponent* UTPSIndicatorDescriptor::GetTargetSceneComponent() const
{
	return TargetSceneComponent;
}

void UTPSIndicatorDescriptor::SetSceneComponent(USceneComponent* InSceneComponent)
{
	TargetSceneComponent = InSceneComponent;
}

FName UTPSIndicatorDescriptor::GetComponentSocketName() const
{
	return ComponentSocketName;
}

void UTPSIndicatorDescriptor::SetComponentSocketName(FName InSocketName)
{
	ComponentSocketName = InSocketName;
}

TSoftClassPtr<UUserWidget> UTPSIndicatorDescriptor::GetIndicatorClass() const
{
	return IndicatorWidgetClass;
}

void UTPSIndicatorDescriptor::SetIndicatorClass(TSoftClassPtr<UUserWidget> InIndicatorClass)
{
	IndicatorWidgetClass = InIndicatorClass;
}

UTPSIndicatorManagerComponent* UTPSIndicatorDescriptor::GetIndicatorManagerComponent() const
{
	return IndicatorManagerComp.Get();
}

void UTPSIndicatorDescriptor::SetIndicatorManagerComponent(UTPSIndicatorManagerComponent* InManager)
{
	if (ensure(IndicatorManagerComp.IsExplicitlyNull()))
	{
		IndicatorManagerComp = InManager;
	}
}

void UTPSIndicatorDescriptor::UnregisterIndicator()
{
}
