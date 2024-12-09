// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacterAnimInstance.h"

#include "TPSHelper.h"
#include "TPSPlayer.h"
#include "Game/Action/TPSActionComponent.h"
#include "Components/TPSAttributeComponent.h"

void UTPSCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UTPSCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (ATPSCharacter* Player = Cast<ATPSCharacter>(GetOwningActor()))
	{
		ActionComp = Player->GetComponentByClass<UTPSActionComponent>();
		AttributeComp = Player->GetComponentByClass<UTPSAttributeComponent>();

		UpdateIsAlive(AttributeComp);
	}
}

void UTPSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateIsAlive(AttributeComp);
}

void UTPSCharacterAnimInstance::UpdateIsAlive(const UTPSAttributeComponent* Attribute)
{
	if (Attribute != nullptr)
	{
		bIsAlive = Attribute->IsAlive();
	}
}
