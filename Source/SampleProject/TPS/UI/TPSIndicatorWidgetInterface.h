// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "UObject/Interface.h"
#include "TPSIndicatorWidgetInterface.generated.h"

class UTPSIndicatorDescriptor;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTPSIndicatorWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLEPROJECT_API ITPSIndicatorWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	void BindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor);
	void UnbindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor);
};
