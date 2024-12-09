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

	if (APlayerController* PlayerController = TPSHelper::GetPlayerController())
	{
		if (ATPSCharacter* Player = Cast<ATPSCharacter>(PlayerController->GetPawn()))
		{
			ActionComp = Player->GetComponentByClass<UTPSActionComponent>();
			AttributeComp = Player->GetComponentByClass<UTPSAttributeComponent>();

			UpdateIsAlive(AttributeComp);
		}
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
		UE_LOG(LogTemp, Log, TEXT("Owner : %s, UpdateIsAlive : %d"), *GetNameSafe(GetOwningActor()), (bIsAlive ? 1:0));
	}
}

