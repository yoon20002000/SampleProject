// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Input/TPSEnhancedInputComponent.h"

UTPSEnhancedInputComponent::UTPSEnhancedInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UTPSEnhancedInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Bind : BindHandles)
	{
		RemoveBindingByHandle(Bind);
	}
	BindHandles.Reset();
}
