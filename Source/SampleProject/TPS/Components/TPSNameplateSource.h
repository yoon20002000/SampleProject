// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TPSNameplateSource.generated.h"


enum class EActorCanvasProjectionMode : uint8;
class UTPSIndicatorDescriptor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSNameplateSource : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSNameplateSource(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Nameplate")
	bool bUseNameplate;
	UPROPERTY(EditDefaultsOnly, Category="Nameplate")
	TSoftClassPtr<UUserWidget> NameplateWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Nameplate")
	EActorCanvasProjectionMode ProjectionMode;
	UPROPERTY(EditDefaultsOnly, Category="Nameplate")
	FName SocketName;
	UPROPERTY(EditDefaultsOnly, Category="Nameplate")
	FVector BoundingBoxAnchor;
	UPROPERTY(EditDefaultsOnly, Category="Nameplate")
	TEnumAsByte<EHorizontalAlignment>  HorizontalAlignment;
	UPROPERTY(EditDefaultsOnly, Category="Nameplate")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;
};
