// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"
#include "TPSIndicatorLayer.generated.h"
class STPSActorCanvas;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSIndicatorLayer : public UWidget
{
	GENERATED_BODY()
protected:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	FSlateBrush ArrowBrush;
protected:
	TSharedPtr<STPSActorCanvas> ActorCanvas;
};
