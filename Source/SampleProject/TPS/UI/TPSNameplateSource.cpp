// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSNameplateSource.h"


// Sets default values for this component's properties
UTPSNameplateSource::UTPSNameplateSource()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTPSNameplateSource::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTPSNameplateSource::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

