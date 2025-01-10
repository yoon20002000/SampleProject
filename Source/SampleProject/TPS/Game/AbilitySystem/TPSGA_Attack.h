#pragma once

#include "Game/TPSGameplayAbility.h"
#include "TPSGA_Attack.generated.h"
UENUM(BlueprintType)
enum class ETPSAbilityTargetingSource : uint8
{
	CameraTowardsFocus,
	PawnForward,
	PawnTowardsFocus,
	WeaponForward,
	WeaponTowardsFocus,
	Custom
};
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSGA_Attack : public UTPSGameplayAbility
{
	GENERATED_BODY()
public:
	UTPSGA_Attack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Ability")
	AController* GetControllerFromActorInfo() const;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		// Weapon Data 추가 시
		// UTPSRangedWeaponInstance*
		
		bool bCanPlayBulletFX;

		FRangedWeaponFiringInput() :
			StartTrace(ForceInitToZero),
			EndAim(ForceInitToZero),
			AimDir(ForceInitToZero),
			bCanPlayBulletFX(false)
		{
			
		}
	};

protected:
	static int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults);

	FHitResult WeaponTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius, bool bIsSimulated,
	                       OUT TArray<FHitResult>& OutHitResults) const;

	FHitResult SingleBulletTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius,
	                             bool bIsSimulated, OUT TArray<FHitResult>& OutHitResults) const;

	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, OUT TArray<FHitResult>& OutHitResults);

	virtual void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	virtual ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;
	void PerformLocalTargeting(OUT TArray<FHitResult>& OutHitResults);
	FVector GetWeaponTargetingSourceLocation() const;
	FTransform GetTargetingTransform(APawn* SourcePawn, ETPSAbilityTargetingSource Source) const;
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag);

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnRangedWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(allowPrivateAccess=true))
	TObjectPtr<UAnimMontage> PlayMontage;
	
	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TSubclassOf<UGameplayEffect>> ToTargetGEs;

	UPROPERTY(EditDefaultsOnly,meta = (allowPrivateAccess=true))
	TObjectPtr<USoundCue> ShotSoundCue;
};
