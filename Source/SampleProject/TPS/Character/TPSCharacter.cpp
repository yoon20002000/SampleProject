// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"

#include "Components/TPSHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"
#include "Game/AbilitySystem/TPSCombatAttributeSet.h"
#include "System/TPSAbilitySet.h"
#include "UI/TPSFloatingHPBar.h"

ATPSCharacter::ATPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UTPSHealthComponent>(TEXT("Attribute Component"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar Widget"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0, 0, HPBarWidgetHeight));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HPBAR(TEXT("/Game/Blueprints/UI/WBP_HPBar.WBP_HPBar_C"));
	if (UI_HPBAR.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HPBAR.Class);
		HPBarWidget->SetDrawSize(HPBarWidgetSize);
	}
	
	AbilitySystemComp = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("AbilitySystem Comp"));
	CombatSet = CreateDefaultSubobject<UTPSCombatAttributeSet>(TEXT("Combat Set"));

	ATPSCharacter::InitializeAttributes();
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATPSCharacter::OnHealthChanged(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue,
	AActor* InstigatorActor)
{
	FString InstigatorActorName = InstigatorActor != nullptr ? InstigatorActor->GetName() : TEXT("null");
	UE_LOG(LogTemp, Log, TEXT("Instigator Actor : %s, OwningComp : %s, NewHealth : %f, Delta : %f"),
		   *InstigatorActorName, *HealthComponent->GetName(), NewValue, NewValue - OldValue);
}

void ATPSCharacter::InitializeAttributes()
{
	if (AbilitySystemComp == nullptr)
	{
		return;
	}
	
	AbilitySystemComp->AddAttributeSetSubobject<UTPSCombatAttributeSet>(CombatSet);
}

void ATPSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (auto AbilitySet : AbilitySets)
	{
		if (AbilitySet != nullptr)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComp, nullptr);
		}
	}
	
	HealthComp->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);

	HPBarWidget->InitWidget();
	UTPSFloatingHPBar* CharacterWidget = Cast<UTPSFloatingHPBar>(HPBarWidget->GetUserWidgetObject());
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindCharacter(this);
	}

	OnAbilitySystemInitialized();
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATPSCharacter::GetHealth() const
{
	return HealthComp->GetHealth();
}

float ATPSCharacter::GetMaxHealth() const
{
	return HealthComp->GetMaxHealth();
}

UTPSHealthComponent* ATPSCharacter::GetAttributeComp()
{
	return HealthComp;
}

UAbilitySystemComponent* ATPSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

UTPSAbilitySystemComponent* ATPSCharacter::GetTPSAbilitySystemComponent() const
{
	return Cast<UTPSAbilitySystemComponent>(GetAbilitySystemComponent());
}

void ATPSCharacter::OnAbilitySystemInitialized()
{
	UTPSAbilitySystemComponent* ASComp = GetTPSAbilitySystemComponent();
	if (ASComp != nullptr)
	{
		HealthComp->Initialize(ASComp);	
	}

	// Init Tag 
}





