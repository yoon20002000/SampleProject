// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSNameplateManager.h"

#include "TPSHelper.h"
#include "Character/TPSPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/TPSIndicatorManagerComponent.h"
#include "UI/TPSIndicatorDescriptor.h"


UTPSNameplateManager::UTPSNameplateManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTPSNameplateManager::RegistNameplate(
	APawn* IndicatedPawn,
	TSoftClassPtr<UUserWidget> InIndicatorClass,
	EActorCanvasProjectionMode InProjectionMode,
	FName InSocketName,
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
	if (USceneComponent* TargetScene = IndicatedPawn->GetComponentByClass<UMeshComponent>())
	{
		NewIndicatorDescriptor->SetTargetSceneComponent(TargetScene);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nameplate Rule Target has not UCapsuleComponent!! : %s "), *IndicatedPawn->GetName());
		return;
	}
	NewIndicatorDescriptor->SetDataObject(IndicatedPawn);
	NewIndicatorDescriptor->SetIndicatorClass(InIndicatorClass);
	NewIndicatorDescriptor->SetProjectionMode(InProjectionMode);
	NewIndicatorDescriptor->SetComponentSocketName(InSocketName);
	NewIndicatorDescriptor->SetBoundingBoxAnchor(InBoundingBoxAnchor);
	NewIndicatorDescriptor->SetHorizontalAlign(InHAlign);
	NewIndicatorDescriptor->SetVerticalAlign(InVAlgin);
	NewIndicatorDescriptor->SetAutoRemoveWhenIndicatorComponentIsNull(bCanAutomaticallyRemove);
	NewIndicatorDescriptor->SetClampToScreen(bIsClampToScreen);
	
	IndicatorMap.Add(IndicatedPawn, NewIndicatorDescriptor);
	
	if (ATPSPlayerController* PC = Cast<ATPSPlayerController>(TPSHelper::GetPlayerController()))
	{
		if (UTPSIndicatorManagerComponent* MG = PC->GetComponentByClass<UTPSIndicatorManagerComponent>())
		{
			MG->AddIndicator(NewIndicatorDescriptor);
		}
	}
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
}

void UTPSNameplateManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (TTuple<TWeakObjectPtr<APawn>, TObjectPtr<UTPSIndicatorDescriptor>> IndicatorDescriptor : IndicatorMap )
	{
		IndicatorDescriptor.Value->UnregisterIndicator();
	}
}
