// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSNameplateManager.h"

#include "Character/TPSPlayer.h"
#include "Components/CapsuleComponent.h"
#include "UI/TPSIndicatorDescriptor.h"


UTPSNameplateManager::UTPSNameplateManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	IndicatorMap.Reserve(8);
}

void UTPSNameplateManager::RegistNameplate(
	APawn* IndicatedPawn,
	TSoftClassPtr<UUserWidget> InIndicatorClass,
	EActorCanvasProjectionMode InProjectionMode,
	FVector InBoundingBoxAnchor,
	EHorizontalAlignment InHAlign,
	EVerticalAlignment InVAlgin,
	bool bCanAutomaticallyRemove,
	bool bIsClampToScreen
	)
{
	if (IndicatedPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Target Pawn is nullptr!!!"));
		return;
	}
	
	UTPSIndicatorDescriptor* NewIndicatorDescriptor = NewObject<UTPSIndicatorDescriptor>();
	if (USceneComponent* TargetScene = IndicatedPawn->GetComponentByClass<UCapsuleComponent>())
	{
		NewIndicatorDescriptor->SetTargetSceneComponent(TargetScene);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nameplate Rule Target has not UCapsuleComponent!! : %s "), *IndicatedPawn->GetName());
		return;
	}
	
	NewIndicatorDescriptor->SetIndicatorClass(InIndicatorClass);
	NewIndicatorDescriptor->SetProjectionMode(InProjectionMode);
	NewIndicatorDescriptor->SetBoundingBoxAnchor(InBoundingBoxAnchor);
	NewIndicatorDescriptor->SetHorizontalAlign(InHAlign);
	NewIndicatorDescriptor->SetVerticalAlign(InVAlgin);
	NewIndicatorDescriptor->SetAutoRemoveWhenIndicatorComponentIsNull(bCanAutomaticallyRemove);
	NewIndicatorDescriptor->SetClampToScreen(bIsClampToScreen);
	
	IndicatorMap.Add(IndicatedPawn, NewIndicatorDescriptor);
}

void UTPSNameplateManager::UnregistNameplate(APawn* IndicatedPawn)
{

	if (auto RemoveItem = IndicatorMap.Find(IndicatedPawn))
	{
		IndicatorMap.Remove(IndicatedPawn);
	
		RemoveItem->Get()->UnregisterIndicator();
	}
}


void UTPSNameplateManager::BeginPlay()
{
	Super::BeginPlay();

	ATPSPlayer* OwnerActor = Cast<ATPSPlayer>(GetOwner());
	if (OwnerActor == nullptr)
	{
		if (AActor* OtherActor = Cast<AActor>(GetOwner()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is not PlayerActor %s!! Auto Destroy Comp!"), *OtherActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner is nullptr!! Auto Destroy Comp!"));
		}
		DestroyComponent();
	}
}

void UTPSNameplateManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for (TTuple<TWeakObjectPtr<APawn>, TObjectPtr<UTPSIndicatorDescriptor>> IndicatorDescriptor : IndicatorMap )
	{
		IndicatorDescriptor.Value->UnregisterIndicator();
	}
}

