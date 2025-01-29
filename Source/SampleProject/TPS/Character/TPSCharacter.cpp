// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"

#include "TPSAIController.h"
#include "TPSPlayer.h"
#include "TPSPlayerState.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSNameplateSource.h"
#include "Components/WidgetComponent.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"
#include "Game/AbilitySystem/TPSCombatAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/TPSAbilitySet.h"
#include "UI/TPSFloatingHPBar.h"

ATPSCharacter::ATPSCharacter() : bCanBeDamaged(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UTPSHealthComponent>(TEXT("Attribute Component"));
	AbilitySystemComp = CreateDefaultSubobject<UTPSAbilitySystemComponent>(TEXT("AbilitySystem Comp"));
	CombatSet = CreateDefaultSubobject<UTPSCombatAttributeSet>(TEXT("Combat Set"));

	NameplateSource = CreateDefaultSubobject<UTPSNameplateSource>(TEXT("Nameplate Source Data"));
	ATPSCharacter::InitializeAttributes();
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterState(ECharacterState::READY);
}

void ATPSCharacter::OnDeathStart(AActor* OwningActor)
{
	if (OwningActor != this)
	{
		UE_LOG(LogTemp, Log, TEXT("Actor not matching!! %s %s "), *GetNameSafe(OwningActor), *GetNameSafe(this));
		return;
	}
	SetCharacterState(ECharacterState::DEAD);
}

void ATPSCharacter::OnHealthChanged(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue,
                                    AActor* InstigatorActor)
{
	FString InstigatorActorName = InstigatorActor != nullptr ? InstigatorActor->GetName() : TEXT("null");
	// UE_LOG(LogTemp, Log, TEXT("Instigator Actor : %s, OwningComp : %s, NewHealth : %f, Delta : %f"),
	// 	   *InstigatorActorName, *HealthComponent->GetName(), NewValue, NewValue - OldValue);
}

void ATPSCharacter::InitializeAttributes()
{
	if (AbilitySystemComp == nullptr)
	{
		return;
	}
	
	AbilitySystemComp->AddAttributeSetSubobject<UTPSCombatAttributeSet>(CombatSet);
}

void ATPSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UCharacterMovementComponent* CMC = GetCharacterMovement();
	bIsPlayerControlled = IsPlayerControlled(); 
	if (bIsPlayerControlled == true)
	{
		bUseControllerRotationYaw = true;
		CMC->bUseControllerDesiredRotation = false;
		CMC->bOrientRotationToMovement = false;
		CMC->RotationRate = FRotator(0.0f,720.f,0.0f);
		CMC->MaxWalkSpeed = 600.0f;
	}
	else
	{
		bUseControllerRotationYaw = false;
		CMC->bUseControllerDesiredRotation = false;
		CMC->bOrientRotationToMovement = true;
		CMC->RotationRate = FRotator(0.0f,480.f,0.0f);
		CMC->MaxWalkSpeed = 300.0f;
	}
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
	HealthComp->OnDeathStart.AddDynamic(this, &ThisClass::OnDeathStart);

	OnAbilitySystemInitialized();
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATPSCharacter::IsAlive() const
{
	return HealthComp->IsAlive();
}

float ATPSCharacter::GetHealth() const
{
	return HealthComp->GetHealth();
}

float ATPSCharacter::GetMaxHealth() const
{
	return HealthComp->GetMaxHealth();
}

UTPSHealthComponent* ATPSCharacter::GetHealthAttributeComp()
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

bool ATPSCharacter::GetCooldownRemainingForTag(const FGameplayTagContainer& CooldownTags, float& RemainingTime,
	float& CooldownDuration) const
{
	if (IsValid(AbilitySystemComp) == true && CooldownTags.Num() > 0)
	{
		RemainingTime = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray<TPair<float,float>> DurationAndTimeRemaining = AbilitySystemComp->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); Idx++)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}
			RemainingTime = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;
			return true;
		}
	}
	return false;
}

void ATPSCharacter::SetCharacterState(ECharacterState CharacterState)
{
	if (CurrentState == CharacterState)
	{
		return;
	}
	CurrentState = CharacterState;

	switch (CurrentState)
	{
	case ECharacterState::PREINIT:
		{
			bCanBeDamaged=false;
			SetPlayerInput(false);
			break;
		}
	case ECharacterState::LOADING:
		{
			bCanBeDamaged=false;
			SetPlayerInput(false);
			break;
		}
	case ECharacterState::READY:
		{
			bCanBeDamaged=true;
			SetPlayerInput(true);
			break;
		}
	case ECharacterState::DEAD:
		{
			AbilitySystemComp->CancelAbilities();
			bCanBeDamaged=false;
			SetPlayerInput(false);
			break;
		}
		default:
			{
				break;
			}
	}
}

ECharacterState ATPSCharacter::GetCharacterState() const
{
	return CurrentState;
}

void ATPSCharacter::UninitAndDestroy()
{
	HealthComp->Uninitialize();
	SetLifeSpan(0.1f);
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


void ATPSCharacter::SetPlayerInput(bool SetActive)
{
	if (bIsPlayerControlled == false)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	check(PlayerController != nullptr);

	if (SetActive == true)
	{
		EnableInput(PlayerController);
	}
	else
	{
		DisableInput(PlayerController);
	}
}
