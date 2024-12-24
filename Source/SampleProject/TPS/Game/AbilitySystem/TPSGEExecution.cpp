#include "Game/AbilitySystem/TPSGEExecution.h"

#include "TPSAbilitySourceInterface.h"
#include "TPSCombatAttributeSet.h"
#include "TPSGameplayEffectContext.h"
#include "TPSHealthSet.h"


struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;
	FDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(UTPSCombatAttributeSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UTPSGEExecution::UTPSGEExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void UTPSGEExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FTPSGameplayEffectContext* TypedContext = FTPSGameplayEffectContext::ExtractEffectContext(Spec.GetContext());
	check(TypedContext);
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseDamage = 0.0f;

	FGameplayEffectAttributeCaptureDefinition DamageStatic = DamageStatics().BaseDamageDef;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic, EvaluateParameters, BaseDamage);

	const AActor* EffectCauser = TypedContext->GetEffectCauser();
	const FHitResult* HitActorResult = TypedContext->GetHitResult();

	AActor* HitActor = nullptr;
	FVector ImpactLocation = FVector::ZeroVector;
	FVector ImpactNormal = FVector::ZeroVector;
	FVector StartTrace = FVector::ZeroVector;
	FVector EndTrace = FVector::ZeroVector;

	// if (HitActorResult != nullptr)
	// {
	// 	const FHitResult& CurHitResult = *HitActorResult;
	// 	HitActor = CurHitResult.HitObjectHandle.FetchActor();
	// 	if (HitActor != nullptr)
	// 	{
	// 		ImpactLocation = HitActor->GetActorLocation();
	// 		ImpactNormal = HitActor->GetActorForwardVector().GetSafeNormal();
	// 		StartTrace = CurHitResult.TraceStart;
	// 		EndTrace = CurHitResult.TraceEnd;
	// 	}
	// }

	if (HitActorResult!= nullptr)
	{
		const FHitResult& CurHitResult = *HitActorResult;
		HitActor = CurHitResult.HitObjectHandle.FetchActor();
		if (HitActor!= nullptr)
		{
			ImpactLocation = CurHitResult.ImpactPoint;
			ImpactNormal = CurHitResult.ImpactNormal;
			StartTrace = CurHitResult.TraceStart;
			EndTrace = CurHitResult.TraceEnd;
		}
	}

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (HitActor != nullptr)
	{
		HitActor = TargetASC != nullptr ? TargetASC->GetAvatarActor_Direct() : nullptr;
		if (HitActor != nullptr)
		{
			ImpactLocation = HitActor->GetActorLocation();
		}
	}

	double Distance = WORLD_MAX;

	if (TypedContext->HasOrigin() == true)
	{
		Distance = FVector::Dist(TypedContext->GetOrigin(), ImpactLocation);
	}
	else if (EffectCauser != nullptr)
	{
		Distance = FVector::Dist(EffectCauser->GetActorLocation(), ImpactLocation);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Damage Calculation Error!!! : %s"), *GetNameSafe(Spec.Def));
	}

	float PhysicalMaterialAttenuation = 1.0f;
	float DistanceAttenuation = 1.0f;

	if (const ITPSAbilitySourceInterface* AbilitySource = TypedContext->GetAbilitySource())
	{
		if (const UPhysicalMaterial* PhysicalMaterial = TypedContext->GetPhysicalMaterial())
		{
			PhysicalMaterialAttenuation = AbilitySource->GetPhysicalMaterialAttenuation(PhysicalMaterial, SourceTags, TargetTags);
		}
		DistanceAttenuation = AbilitySource->GetDistanceAttenuation(Distance, SourceTags, TargetTags);
	}
	DistanceAttenuation = FMath::Max(DistanceAttenuation, 0.0f);

	const float DamageDone = FMath::Max(BaseDamage * DistanceAttenuation * PhysicalMaterialAttenuation, 0.0f);
	
	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UTPSHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
	}
#endif
}
