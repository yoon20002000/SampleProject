// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSProjectileMovementComponent.h"

#include "System/TickablesSubsystem.h"

void UTPSProjectileMovementComponent::Reset()
{
	Velocity = FVector(1,0,0);

	InitializeComponent();
}

void UTPSProjectileMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UTickablesSubsystem* Tickables = GetWorld()->GetSubsystem<UTickablesSubsystem>())
	{
		Tickables->RegisterComponent(&PrimaryComponentTick);	
	}
}

void UTPSProjectileMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UTickablesSubsystem* Tickables = GetWorld()->GetSubsystem<UTickablesSubsystem>())
	{
		Tickables->UnregisterComponent(&PrimaryComponentTick);
	}
}
