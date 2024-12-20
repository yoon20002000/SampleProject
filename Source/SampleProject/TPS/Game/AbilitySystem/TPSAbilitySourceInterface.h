#pragma once

#include "UObject/Interface.h"

#include "TPSAbilitySourceInterface.generated.h"


struct FGameplayTagContainer;

UINTERFACE(MinimalAPI)
class UTPSAbilitySourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SAMPLEPROJECT_API ITPSAbilitySourceInterface
{
	GENERATED_BODY()
	
public:
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const = 0;
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial,
	                                             const FGameplayTagContainer* SourceTags = nullptr,
	                                             const FGameplayTagContainer* TargetTags = nullptr) const =0;
};
