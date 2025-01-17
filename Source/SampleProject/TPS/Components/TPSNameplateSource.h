// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TPSNameplateSource.generated.h"


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
	TSoftClassPtr<UUserWidget> NameplateWidgetClass;
};
