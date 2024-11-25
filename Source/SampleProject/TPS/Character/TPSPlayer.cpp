#include "TPSPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TPSPlayerController.h"
#include "Components/InteractionComponent.h"
#include "Components/TPSCameraComponent.h"
#include "Components/TPSSpringArmComponent.h"
#include "Game/Action/ActionComponent.h"
#include "Game/Action/SharedGameplayTags.h"

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

	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Comp"));
	ActionComp = CreateDefaultSubobject<UActionComponent>(TEXT("Action Comp"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	SpringArmComp = FindComponentByClass<UTPSSpringArmComponent>();
	CameraComp = FindComponentByClass<UTPSCameraComponent>();
}

void ATPSPlayer::OnHealthChanged(AActor* InstigatorActor, UTPSAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	Super::OnHealthChanged(InstigatorActor, OwningComp, NewHealth, Delta);

	if (NewHealth <= 0)
	{
		if (ATPSPlayerController* TPSPC = Cast<ATPSPlayerController>(GetController()))
		{
			TPSPC->SetGameEnd();
		}
	}
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

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSPlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(ShotAction, ETriggerEvent::Started, this, &ATPSPlayer::Shot);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATPSPlayer::Interaction);
	}
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

void ATPSPlayer::Shot(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Log, TEXT("Shot!!"));
	AttributeComp->ApplyHealthChange(this,-1);
	Attack();
}
void ATPSPlayer::Interaction(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Log, TEXT("Interaction"));
	InteractionComp->Interaction();
}

void ATPSPlayer::Attack()
{
	ActionComp->StartActionByName(this, SharedGameplayTags::Action_Attack);
}
