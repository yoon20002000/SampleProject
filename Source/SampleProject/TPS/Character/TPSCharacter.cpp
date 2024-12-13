// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"

#include "Components/TPSAttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/TPSGameplayAbility.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"
#include "Game/Action/TPSActionComponent.h"
#include "Game/Action/TPSSharedGameplayTags.h"
#include "UI/TPSFloatingHPBar.h"

ATPSCharacter::ATPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComp = CreateDefaultSubobject<UTPSAttributeComponent>(TEXT("Attribute Component"));

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

	ActionComp = CreateDefaultSubobject<UTPSActionComponent>(TEXT("Action Comp"));
	AbilitySystemComp = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("AbilitySystem Comp"));
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATPSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ATPSCharacter::OnHealthChanged);

	HPBarWidget->InitWidget();
	UTPSFloatingHPBar* CharacterWidget = Cast<UTPSFloatingHPBar>(HPBarWidget->GetUserWidgetObject());
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindCharacter(this);
	}
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATPSCharacter::GetHealth() const
{
	return AttributeComp->GetHealth();
}

float ATPSCharacter::GetMaxHealth() const
{
	return AttributeComp->GetMaxHealth();
}

void ATPSCharacter::AddHP(const int InValue)
{
	AttributeComp->ApplyHealthChange(this, InValue);
}

UTPSAttributeComponent* ATPSCharacter::GetAttributeComp()
{
	return AttributeComp;
}

UAbilitySystemComponent* ATPSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ATPSCharacter::AddAbilities()
{
	if (AbilitySystemComp == nullptr)
	{
		return;
	}

	// for (TSubclassOf<UTPSGameplayAbility>& Ability : CharacterAbilities)
	// {
	// 	UTPSGameplayAbility* AbilityCDO = Ability->GetDefaultObject<UTPSGameplayAbility>();
	// 	FGameplayAbilitySpec AbilitySpec(AbilityCDO, 1);
	// 	AbilitySpec.SourceObject = SourceObject;
	// 	AbilitySpec.GetDynamicSpecSourceTags().AddTag(Ability->)
	// 	
	// 	AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(Ability,GetAbilityLevel(),,this));
	// }
}

void ATPSCharacter::OnHealthChanged(AActor* InstigatorActor, UTPSAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	UE_LOG(LogTemp, Log, TEXT("Instigator Actor : %s, OwningComp : %s, NewHealth : %f, Delta : %f"),
	       *InstigatorActor->GetName(), *OwningComp->GetName(), NewHealth, Delta);

	// 추후 죽는 ani 추가
	if (NewHealth < 0)
	{
		SetLifeSpan(5.0f);
	}
}

void ATPSCharacter::Attack()
{
	ActionComp->StartActionByName(this, TPSSharedGameplayTags::Action_Attack);
}
