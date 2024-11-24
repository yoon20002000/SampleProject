// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSAttributeComponent.h"

#include "TPSHelper.h"
#include "Game/TPSGameMode.h"

// Sets default values for this component's properties
UTPSAttributeComponent::UTPSAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Health = MaxHealth;

	Mana = MaxMana;
	UE_LOG(LogTemp, Log,TEXT("Construct %f"), MaxHealth);
}

void UTPSAttributeComponent::OnRegister()
{
	Super::OnRegister();
	Health = MaxHealth;
}

void UTPSAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

UTPSAttributeComponent* UTPSAttributeComponent::GetAttributes(AActor* InActor)
{
	if (InActor == nullptr)
	{
		return nullptr;
	}

	return InActor->FindComponentByClass<UTPSAttributeComponent>();
}

bool UTPSAttributeComponent::IsActorAlive(AActor* InActor)
{
	UTPSAttributeComponent* ActorAttributeComp = GetAttributes(InActor);

	if (ActorAttributeComp == nullptr)
	{
		return false;
	}

	return ActorAttributeComp->IsAlive();
}

void UTPSAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
                                                                   float ActuableDelta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, ActuableDelta);
}

void UTPSAttributeComponent::MulticastManaChanged_Implementation(AActor* InstigatorActor, float NewMana,
                                                                 float ActuableDelta)
{
	OnManaChanged.Broadcast(InstigatorActor, this, NewMana, ActuableDelta);
}

bool UTPSAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float UTPSAttributeComponent::GetHealth() const
{
	return Health;
}

float UTPSAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UTPSAttributeComponent::GetMana() const
{
	return Mana;
}

float UTPSAttributeComponent::GetMaxMana() const
{
	return MaxMana;
}

bool UTPSAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, const float Delta)
{
	if (GetOwner()->CanBeDamaged() == false && Delta < 0.0f)
	{
		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0, MaxHealth);

	float ActuableDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority() == true)
	{
		Health = NewHealth;

		if (FMath::IsNearlyZero(ActuableDelta) == false)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActuableDelta);
		}

		if (ActuableDelta < 0.0f && FMath::IsNearlyZero(Health))
		{
			ATPSGameMode* GM = TPSHelper::GetWorld()->GetAuthGameMode<ATPSGameMode>();
			if (GM != nullptr)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	return FMath::IsNearlyZero(ActuableDelta) == false;
}

bool UTPSAttributeComponent::ApplyManaChange(AActor* InstigatorActor, const float Delta)
{
	const float OldMana = Mana;

	Mana = FMath::Clamp(Mana + Delta, 0, MaxMana);

	float ActualDelta = Mana - OldMana;

	if (FMath::IsNearlyZero(ActualDelta) == false)
	{
		OnManaChanged.Broadcast(InstigatorActor, this, Mana, ActualDelta);
		return true;
	}
	return false;
}
