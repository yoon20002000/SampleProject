// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSHealthComponent.h"

#include "Game/TPSGameplayTags.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"
#include "Game/AbilitySystem/TPSHealthSet.h"

UTPSHealthComponent::UTPSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UTPSHealthComponent::OnRegister()
{
	Super::OnRegister();
	
}

void UTPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTPSHealthComponent::Initialize(UTPSAbilitySystemComponent* InASComp)
{
	if (AActor* Owner = GetOwner(); Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Have not owner!!"));
		return;
	}
	
	if (InASComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeComp Error : Owner ability system is nullptr!!!"));
		return;
	}

	if (AbilitySystemComp != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeComp Error : Already Initialized"));
		return;
	}
	
	AbilitySystemComp = InASComp;

	HealthAttributeSet = AbilitySystemComp->GetSet<UTPSHealthSet>();

	if (HealthAttributeSet == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Health Attribute Set is nullptr!!"));
		return;
	}

	HealthAttributeSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthAttributeSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthAttributeSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	AbilitySystemComp->SetNumericAttributeBase(UTPSHealthSet::GetHealthAttribute(), HealthAttributeSet->GetHealth());
	
	ClearGameplayTags();
	OnHealthChanged.Broadcast(this, HealthAttributeSet->GetHealth(),HealthAttributeSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthAttributeSet->GetMaxHealth(),HealthAttributeSet->GetMaxHealth(), nullptr);
}

void UTPSHealthComponent::Uninitialize()
{
	ClearGameplayTags();
	if (HealthAttributeSet != nullptr)
	{
		HealthAttributeSet->OnHealthChanged.RemoveAll(this);
		HealthAttributeSet->OnMaxHealthChanged.RemoveAll(this);
		HealthAttributeSet->OnOutOfHealth.RemoveAll(this);
	}

	HealthAttributeSet = nullptr;
	AbilitySystemComp = nullptr;
}

float UTPSHealthComponent::GetHealth() const
{
	if (HealthAttributeSet == nullptr)
	{
		return -1.0f;
	}
	return HealthAttributeSet->GetHealth();
}

float UTPSHealthComponent::GetMaxHealth() const
{
	if (HealthAttributeSet == nullptr)
	{
		return -1.0f;
	}
	return HealthAttributeSet->GetMaxHealth();
}

float UTPSHealthComponent::GetHealthPercentage() const
{
	if (HealthAttributeSet == nullptr)
	{
		return -1.0f;
	}
	return HealthAttributeSet->GetHealth() / HealthAttributeSet->GetMaxHealth();
}

bool UTPSHealthComponent::IsAlive() const
{
	if (HealthAttributeSet == nullptr)
	{
		return false;
	}
	
	return HealthAttributeSet->GetHealth() > 0;
}

void UTPSHealthComponent::ClearGameplayTags()
{
	if (AbilitySystemComp == nullptr)
	{
		return;
	}

	AbilitySystemComp->SetLooseGameplayTagCount(TPSGameplayTags::Status_Death_Dying, 0);
	AbilitySystemComp->SetLooseGameplayTagCount(TPSGameplayTags::Status_Death_Death, 0);
}


void UTPSHealthComponent::HandleHealthChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* GameplayEffectSpec,
                                              float DamageMagnitude, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this,OldValue,NewValue, DamageInstigator);	
}

void UTPSHealthComponent::HandleMaxHealthChanged(AActor* Instigator, AActor* DamageCauser,
	const FGameplayEffectSpec* GameplayEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this,OldValue,NewValue, Instigator);
}

void UTPSHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* GameplayEffectSpec,
	float DamageMagnitude, float OldValue, float NewValue)
{
	// 추가 작업 필요
	UE_LOG(LogTemp, Log, TEXT("Death!!!!!!"));
}
