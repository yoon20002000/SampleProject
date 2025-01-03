﻿#include "TPSPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/TPSInteractionComponent.h"
#include "Components/TPSCameraComponent.h"
#include "Components/TPSSpringArmComponent.h"
#include "Game/TPSGameplayTags.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"
#include "System/Input/TPSEnhancedInputComponent.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<UTPSSpringArmComponent>(TEXT("CustomSpringArm"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetUsingAbsoluteRotation(true);

	CameraComp = CreateDefaultSubobject<UTPSCameraComponent>(TEXT("CustomCamera"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UTPSInteractionComponent>(TEXT("Interaction Comp"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	SpringArmComp = FindComponentByClass<UTPSSpringArmComponent>();
	CameraComp = FindComponentByClass<UTPSCameraComponent>();
}

void ATPSPlayer::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (AbilitySystemComp == nullptr)
	{
		return;
	}

	AbilitySystemComp->AbilityInputTagPressed(InputTag);
}

void ATPSPlayer::AbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComp->AbilityInputTagReleased(InputTag);
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	UTPSEnhancedInputComponent* TPSEIComp = Cast<UTPSEnhancedInputComponent>(PlayerInputComponent);
	if (TPSEIComp != nullptr)
	{
		TPSEIComp->BindNativeAction(InputConfig, TPSGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this,
		                            &ATPSPlayer::Move);
		TPSEIComp->BindNativeAction(InputConfig, TPSGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this,
		                            &ATPSPlayer::Look);
		TArray<uint32> BindHandles;
		TPSEIComp->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased,BindHandles);
		
		TPSEIComp->BindAction(InteractAction, ETriggerEvent::Started, this, &ATPSPlayer::Interaction);
	}
}

void ATPSPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATPSPlayer::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPSPlayer::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATPSPlayer::Interaction(const FInputActionValue& InputActionValue)
{
	InteractionComp->Interaction();
}

void ATPSPlayer::JumpAbilities()
{
	FGameplayTagContainer JumpTags;
	JumpTags.AddTag(TPSGameplayTags::Action_Jump);

	AbilitySystemComp->TryActivateAbilitiesByTag(JumpTags);
}

void ATPSPlayer::StopJumpAbilities()
{
	FGameplayTagContainer JumpTags;
	JumpTags.AddTag(TPSGameplayTags::Action_Jump);
	AbilitySystemComp->CancelAbilities();
}
