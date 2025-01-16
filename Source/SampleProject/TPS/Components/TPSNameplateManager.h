// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "UI/TPSIndicatorDescriptor.h"
#include "TPSNameplateManager.generated.h"


class ATPSCharacter;
class UTPSIndicatorDescriptor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSNameplateManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSNameplateManager();

	void RegistNameplate(APawn* IndicatedPawn,
							TSoftClassPtr<UUserWidget> InIndicatorClass,
							EActorCanvasProjectionMode InProjectionMode = EActorCanvasProjectionMode::ComponentBoundingBox,
							FVector InBoundingBoxAnchor = FVector::UnitZ(),
							EHorizontalAlignment InHAlign = HAlign_Center,
							EVerticalAlignment InVAlgin = VAlign_Center,
							bool bCanAutomaticallyRemove = true,
							bool bIsClampToScreen = true);
	void UnregistNameplate(APawn* IndicatedPawn);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY()
	TMap<TWeakObjectPtr<APawn>,TObjectPtr<UTPSIndicatorDescriptor>> IndicatorMap;
};
