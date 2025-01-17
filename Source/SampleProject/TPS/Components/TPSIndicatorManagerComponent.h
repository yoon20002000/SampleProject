// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TPSIndicatorManagerComponent.generated.h"



class UTPSIndicatorDescriptor;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLEPROJECT_API UTPSIndicatorManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);

	static UTPSIndicatorManagerComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category="Indicators")
	void AddIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor);
	UFUNCTION(BlueprintCallable, Category="Indicators")
	void RemoveIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor);

	const TArray<UTPSIndicatorDescriptor*>& GetIndicators() const;
	
public:
	DECLARE_EVENT_OneParam(UTPSIndicatorManagerComponent,FIndicatorEvent, UTPSIndicatorDescriptor* IndicatorDescriptor)
	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UTPSIndicatorDescriptor>> Indicators;
};
