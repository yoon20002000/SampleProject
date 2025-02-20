// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacterAnimInstance.h"
#include "TPSPlayer.h"
#include "Components/TPSHealthComponent.h"

void UTPSCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UTPSCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (ATPSCharacter* Player = Cast<ATPSCharacter>(GetOwningActor()))
	{
		AttributeComp = Player->GetHealthComponentOrNullptr();

		if (AttributeComp != nullptr)
		{
			UpdateIsAlive(AttributeComp);	
		}
	}
}

void UTPSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateIsAlive(AttributeComp);
}

void UTPSCharacterAnimInstance::UpdateIsAlive(const UTPSHealthComponent* Attribute)
{
	if (Attribute != nullptr)
	{
		bIsAlive = Attribute->IsAlive();
	}
}
