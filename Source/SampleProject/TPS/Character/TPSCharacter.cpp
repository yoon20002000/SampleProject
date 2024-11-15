﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"

#include "Components/WidgetComponent.h"
// Sets default values
ATPSCharacter::ATPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar Widget"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0,0,HPBarWidgetHeight));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HPBAR(TEXT("/Game/Blueprints/UI/WBP_HPBar.WBP_HPBar_C"));
	if (UI_HPBAR.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HPBAR.Class);
		HPBarWidget->SetDrawSize(HPBarWidgetSize);
	}
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATPSCharacter::GetHealth() const
{
	return Health;
}

float ATPSCharacter::GetMaxHealth() const
{
	return MaxHealth;
}

void ATPSCharacter::ChangeHealth(float InNewHealth)
{
	Health = FMath::Clamp(InNewHealth, 0,MaxHealth);
}
