// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSProjectileMovementComponent.h"

#include "System/TPSTickablesSubsystem.h"

void UTPSProjectileMovementComponent::Reset()
{
	Velocity = FVector(1,0,0);

	InitializeComponent();
}

void UTPSProjectileMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UTPSTickablesSubsystem* Tickables = GetWorld()->GetSubsystem<UTPSTickablesSubsystem>())
	{
		Tickables->RegisterComponent(&PrimaryComponentTick);	
	}
}

void UTPSProjectileMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UTPSTickablesSubsystem* Tickables = GetWorld()->GetSubsystem<UTPSTickablesSubsystem>())
	{
		Tickables->UnregisterComponent(&PrimaryComponentTick);
	}
}
