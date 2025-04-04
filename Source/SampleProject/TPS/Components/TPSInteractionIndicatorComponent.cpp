#include "Components/TPSInteractionIndicatorComponent.h"

#include "TPSHelper.h"
#include "TPSIndicatorManagerComponent.h"
#include "Actor/TPSApplyGEActor.h"
#include "Blueprint/UserWidget.h"
#include "Character/TPSPlayerController.h"
#include "System/TPSCollisionChannels.h"
#include "UI/TPSIndicatorDescriptor.h"

UTPSInteractionIndicatorComponent::UTPSInteractionIndicatorComponent() : InteractionCheckRadius(200.0f),
                                                                         InteractionCheckRange(200.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTPSInteractionIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ATPSApplyGEActor* InteractionActor = GetInteractionActor())
	{
		RegistInteractionIndicator(InteractionActor, InteractionIndicatorClass,
		                           EActorCanvasProjectionMode::ComponentPoint, NAME_None, FVector::UnitZ(),
		                           HAlign_Center, VAlign_Center, true, false);
	}
	else
	{
		UnregistInteractionIndicator();
	}
}

void UTPSInteractionIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTPSInteractionIndicatorComponent::RegistInteractionIndicator(AActor* InteractionActor,
                                                                   TSoftClassPtr<UUserWidget> InIndicatorClass,
                                                                   EActorCanvasProjectionMode InProjectionMode,
                                                                   FName InSocketName,
                                                                   FVector InBoundingBoxAnchor,
                                                                   EHorizontalAlignment InHAlign,
                                                                   EVerticalAlignment InVAlign,
                                                                   bool bCanAutomaticallyRemove, bool bIsClampToScreen)
{
	if (CurIndicator != nullptr)
	{
		return;
	}
	
	if (InteractionActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Target Pawn is nullptr!!!"));
		return;
	}

	UTPSIndicatorDescriptor* NewIndicatorDescriptor = NewObject<UTPSIndicatorDescriptor>();
	if (USceneComponent* TargetScene = InteractionActor->GetComponentByClass<UMeshComponent>())
	{
		NewIndicatorDescriptor->SetTargetSceneComponent(TargetScene);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nameplate Rule Target has not UCapsuleComponent!! : %s "),
		       *InteractionActor->GetName());
		return;
	}
	NewIndicatorDescriptor->SetDataObject(InteractionActor);
	NewIndicatorDescriptor->SetIndicatorClass(InIndicatorClass);
	NewIndicatorDescriptor->SetProjectionMode(InProjectionMode);
	NewIndicatorDescriptor->SetComponentSocketName(InSocketName);
	NewIndicatorDescriptor->SetBoundingBoxAnchor(InBoundingBoxAnchor);
	NewIndicatorDescriptor->SetHorizontalAlign(InHAlign);
	NewIndicatorDescriptor->SetVerticalAlign(InVAlign);
	NewIndicatorDescriptor->SetAutoRemoveWhenIndicatorComponentIsNull(bCanAutomaticallyRemove);
	NewIndicatorDescriptor->SetClampToScreen(bIsClampToScreen);

	CurIndicator = NewIndicatorDescriptor;

	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(TPSHelper::GetPlayerController()))
	{
		if (UTPSIndicatorManagerComponent* MG = PC->GetComponentByClass<UTPSIndicatorManagerComponent>())
		{
			MG->AddIndicator(NewIndicatorDescriptor);
		}
	}
}

void UTPSInteractionIndicatorComponent::UnregistInteractionIndicator()
{
	if (CurIndicator == nullptr)
	{
		return;
	}
	
	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(TPSHelper::GetPlayerController()))
	{
		if (UTPSIndicatorManagerComponent* MG = PC->GetComponentByClass<UTPSIndicatorManagerComponent>())
		{
			MG->RemoveIndicator(CurIndicator.Get());
		}
	}
	CurIndicator = nullptr;
}

void UTPSInteractionIndicatorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnregistInteractionIndicator();
}

ATPSApplyGEActor* UTPSInteractionIndicatorComponent::GetInteractionActor() const
{
	AActor* OwnerActor = GetOwner();
	FCollisionQueryParams TraceParams(TEXT("Interaction Indicator Actor"), true, OwnerActor);

	TArray<AActor*> AttachedActors;
	OwnerActor->GetAttachedActors(AttachedActors);
	TraceParams.AddIgnoredActors(AttachedActors);
	TraceParams.AddIgnoredActor(OwnerActor);
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	TraceParams.bDebugQuery = true;
#endif
	

	TArray<FHitResult> HitResults;
	FVector TraceStart;
	FRotator TempRotator;
	OwnerActor->GetActorEyesViewPoint(OUT TraceStart, OUT TempRotator);
	FVector TraceEnd = TraceStart + OwnerActor->GetActorForwardVector() * InteractionCheckRange;

	GetWorld()->LineTraceMultiByChannel(OUT HitResults, TraceStart, TraceEnd,TPS_TraceChannel_Weapon, TraceParams);

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr && HitActor->IsA(ATPSApplyGEActor::StaticClass()) == true)
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Green);
			return Cast<ATPSApplyGEActor>(HitActor);
		}
	}

	return nullptr;
}
