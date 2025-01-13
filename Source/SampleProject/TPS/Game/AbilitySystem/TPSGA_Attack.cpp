#include "Game/AbilitySystem/TPSGA_Attack.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "NativeGameplayTags.h"
#include "TPSAT_PlayMontageAndWaitForEvent.h"
#include "Character/TPSCharacter.h"
#include "Character/TPSPlayer.h"
#include "Components/TPSCameraComponent.h"
#include "Game/TPSGameplayTags.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "System/TPSCollisionChannels.h"
#include "System/TPSGATargetData_SingleTargetHit.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

const FName SocketName = TEXT("Gun_LOS");

FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;

		FRotator Rot = Dir.Rotation();
		FQuat DirQuat(Rot);
		FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuat(FRotator(0.0f, 0.0f, AngleAround));
		FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;

		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}

UTPSGA_Attack::UTPSGA_Attack(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SourceBlockedTags.AddTag(TPSGameplayTags::TAG_WeaponFireBlocked);
}

AController* UTPSGA_Attack::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo != nullptr)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		AActor* Actor = CurrentActorInfo->OwnerActor.Get();
		while (Actor != nullptr)
		{
			if (AController* Controller = Cast<AController>(Actor))
			{
				return Controller;
			}

			if (APawn* Pawn = Cast<APawn>(Actor))
			{
				return Pawn->GetController();
			}
			Actor = Actor->GetOwner();
		}
	}
	return nullptr;
}

void UTPSGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* AC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(AC);


	OnTargetDataReadyCallbackDelegateHandle = AC->AbilityTargetDataSetDelegate(
		CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).AddUObject(
		this, &ThisClass::OnTargetDataReadyCallback);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	AActor* OwnerActor = GetAvatarActorFromActorInfo();
	if (ATPSCharacter* OwnerPawn = Cast<ATPSCharacter>(OwnerActor); PlayMontage != nullptr)
	{
		UGameplayStatics::SpawnSoundAttached(ShotSoundCue, OwnerPawn->GetMesh(), SocketName);

		UTPSAT_PlayMontageAndWaitForEvent* AT = UTPSAT_PlayMontageAndWaitForEvent::CreatePlayMontageAndWaitForEvent(
			this, NAME_None, PlayMontage, FGameplayTagContainer());

		AT->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleted);
		AT->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
		AT->OnInterrupted.AddDynamic(this, &ThisClass::OnCancelled);
		AT->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
		AT->OnEventReceived.AddDynamic(this, &ThisClass::OnReceived);

		AT->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

bool UTPSGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayTagContainer* SourceTags,
                                       const FGameplayTagContainer* TargetTags,
                                       FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
	{
		return false;
	}

	// 이후 무기를 추가시 추가 조건 할 것

	return bResult;
}

void UTPSGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                               bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo) == true)
	{
		if (ScopeLockCount > 0)
		{
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &ThisClass::EndAbility, Handle, ActorInfo,
			                                                      ActivationInfo, bReplicateEndAbility,
			                                                      bWasCancelled));;
			return;
		}

		UAbilitySystemComponent* AC = CurrentActorInfo->AbilitySystemComponent.Get();
		check(AC);

		FAbilityTargetDataSetDelegate& ATDSD = AC->AbilityTargetDataSetDelegate(
			CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
		ATDSD.Remove(OnTargetDataReadyCallbackDelegateHandle);
		AC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

int32 UTPSGA_Attack::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int Index = 0; Index < HitResults.Num(); ++Index)
	{
		const FHitResult& HitResult = HitResults[Index];
		if (HitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()) == true)
		{
			return Index;
		}
		else
		{
			AActor* HitActor = HitResult.HitObjectHandle.FetchActor();
			if (HitActor != nullptr && HitActor->GetAttachParentActor() != nullptr && Cast<APawn>(
				HitActor->GetAttachParentActor()) != nullptr)
			{
				return Index;
			}
		}
	}
	return INDEX_NONE;
}

void UTPSGA_Attack::AttackExecute()
{
	TArray<FHitResult> FoundHits;
	PerformLocalTargeting(FoundHits);

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	// 무기 추가시 수정 필요
	TargetDataHandle.UniqueId = 0;

	if (FoundHits.Num() > 0)
	{
		const int32 CartridgeID = FMath::Rand();

		for (const FHitResult& FoundHit : FoundHits)
		{
			FTPSGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new
				FTPSGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;
			TargetDataHandle.Add(NewTargetData);
		}
	}

	const bool bProjectileWeapon = false;
	if (bProjectileWeapon == false)
	{
		// WeaponStateComponent->AddUnconfirmedServerSideHitMarkers();
	}

	OnTargetDataReadyCallback(TargetDataHandle, FGameplayTag());
}

FHitResult UTPSGA_Attack::WeaponTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius,
                                      bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;
	AddAdditionalTraceIgnoreActors(TraceParams);
	TraceParams.bDebugQuery = true;

	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);

	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, TraceStart, TraceEnd, FQuat::Identity, TraceChannel,
		                                FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, TraceChannel, TraceParams);
	}

	FHitResult Hit(ForceInit);
	if (HitResults.Num() > 0)
	{
		for (FHitResult& HitResult : HitResults)
		{
			auto Pred = [&HitResult](const FHitResult& Other)
			{
				return Other.HitObjectHandle == HitResult.HitObjectHandle;
			};

			if (OutHitResults.ContainsByPredicate(Pred) == false)
			{
				OutHitResults.Add(HitResult);
			}
		}
		Hit = OutHitResults.Last();
	}
	else
	{
		Hit.TraceStart = TraceStart;
		Hit.TraceEnd = TraceEnd;
	}

	return Hit;
}

FHitResult UTPSGA_Attack::SingleBulletTrace(const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius,
                                            bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	// Draw Debug
	{
		float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1, 0, DebugThickness);
	}

	FHitResult Impact;

	// Not use SweepRadius
	if (FindFirstPawnHitResult(OutHitResults) == INDEX_NONE)
	{
		Impact = WeaponTrace(TraceStart, TraceEnd, 0.0f, bIsSimulated, OutHitResults);
	}

	if (FindFirstPawnHitResult(OutHitResults) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(TraceStart, TraceEnd, SweepRadius, bIsSimulated, SweepHits);

			const int32 FirstPawnIndex = FindFirstPawnHitResult(OutHitResults);
			if (SweepHits.IsValidIndex(FirstPawnIndex) == true)
			{
				bool bUseSweepHits = true;
				for (int32 Index = 0; Index < FirstPawnIndex; ++Index)
				{
					const FHitResult& HitResult = SweepHits[Index];

					auto Pred = [&HitResult](const FHitResult& Other)
					{
						return Other.HitObjectHandle == HitResult.HitObjectHandle;
					};
					if (HitResult.bBlockingHit == true && OutHitResults.ContainsByPredicate(Pred) == true)
					{
						bUseSweepHits = false;
						break;
					}
				}
				if (bUseSweepHits == true)
				{
					OutHitResults = SweepHits;
				}
			}
		}
	}
	return Impact;
}

void UTPSGA_Attack::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData,
                                            TArray<FHitResult>& OutHitResults)
{
	const float BaseSpreadAngle = 2.5f;
	const float SpreadAngleMultiplier = 0.902564f;
	const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;

	const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(ActualSpreadAngle * 0.5f);

	constexpr float SpreadExponent = 1.0f;
	constexpr float MaxDamageRange = 25000.0f;
	constexpr float BulletTraceSweepRadius = 6.0f;
	const FVector BulletDir = VRandConeNormalDistribution(InputData.AimDir, HalfSpreadAngleInRadians, SpreadExponent);

	const FVector EndTrace = InputData.StartTrace + (BulletDir * MaxDamageRange);
	FVector HitLocation = EndTrace;

	TArray<FHitResult> AllImpacts;

	FHitResult Impact = SingleBulletTrace(InputData.StartTrace, EndTrace, BulletTraceSweepRadius, false, AllImpacts);

	const AActor* HitActor = Impact.GetActor();

	if (HitActor != nullptr)
	{
		DrawDebugPoint(GetWorld(), Impact.ImpactPoint, 3.0f, FColor::Red, false, 2);

		if (AllImpacts.Num() > 0)
		{
			OutHitResults.Append(AllImpacts);
		}

		HitLocation = Impact.ImpactPoint;
	}

	if (OutHitResults.Num() == 0)
	{
		if (!Impact.bBlockingHit)
		{
			Impact.Location = EndTrace;
			Impact.ImpactPoint = EndTrace;
		}

		OutHitResults.Add(Impact);
	}
}

void UTPSGA_Attack::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;
		Avatar->GetAttachedActors(AttachedActors);
		TraceParams.AddIgnoredActors(AttachedActors);
	}
}

ECollisionChannel UTPSGA_Attack::DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const
{
	return TPS_TraceChannel_Weapon;
}

void UTPSGA_Attack::PerformLocalTargeting(TArray<FHitResult>& OutHitResults)
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);

	if (AvatarPawn->IsLocallyControlled() == true)
	{
		FRangedWeaponFiringInput InputData;
		InputData.bCanPlayBulletFX = AvatarPawn->GetNetMode() != NM_DedicatedServer;

		const FTransform TargetTransform = GetTargetingTransform(
			AvatarPawn, ETPSAbilityTargetingSource::CameraTowardsFocus);
		InputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.StartTrace = TargetTransform.GetTranslation();

		InputData.EndAim = InputData.StartTrace + InputData.AimDir * 10;

		DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir * 100.0f),
		              FColor::Yellow, false, 1.f, 0, 2.0f);

		TraceBulletsInCartridge(InputData, OutHitResults);
	}
}

FVector UTPSGA_Attack::GetWeaponTargetingSourceLocation() const
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);


	// Original Lyra Code
	// const FVector SourceLocation = AvatarPawn->GetActorLocation();
	// const FQuat SourceRotation = AvatarPawn->GetActorQuat();
	//FVector TargetingSourceLocation = SourceLocation;

	USkeletalMeshComponent* SkelMeshComp = AvatarPawn->GetComponentByClass<USkeletalMeshComponent>();
	FVector TargetingSourceLocation = SkelMeshComp->GetSocketLocation(SocketName);

	return TargetingSourceLocation;
}

FTransform UTPSGA_Attack::GetTargetingTransform(APawn* SourcePawn, ETPSAbilityTargetingSource Source) const
{
	check(SourcePawn);
	AController* SourcePawnController = SourcePawn->GetController();

	check(Source != ETPSAbilityTargetingSource::Custom);

	const FVector ActorLocation = SourcePawn->GetActorLocation();
	FQuat AimQuat = SourcePawn->GetActorQuat();
	FVector SourceLocation;

	double FocalDistance = 1024.0f;
	FVector FocallLocation;

	FVector CamLocation;
	FRotator CamRotation;
	bool bFoundFocus = false;


	if (SourcePawnController != nullptr &&
		(
			(Source == ETPSAbilityTargetingSource::CameraTowardsFocus) ||
			(Source == ETPSAbilityTargetingSource::PawnTowardsFocus) ||
			(Source == ETPSAbilityTargetingSource::WeaponTowardsFocus))
	)
	{
		bFoundFocus = true;

		APlayerController* PC = Cast<APlayerController>(SourcePawn->GetController());
		if (PC != nullptr)
		{
			PC->GetPlayerViewPoint(CamLocation, CamRotation);
		}
		else
		{
			SourceLocation = GetWeaponTargetingSourceLocation();
			CamLocation = SourceLocation;
			CamRotation = SourcePawnController->GetControlRotation();
		}

		FVector AimDir = CamRotation.Vector().GetSafeNormal();
		FocallLocation = CamLocation + (AimDir * FocalDistance);

		if (PC != nullptr)
		{
			const FVector WeaponLocation = GetWeaponTargetingSourceLocation();
			CamLocation = FocallLocation + (((WeaponLocation - FocallLocation) | AimDir) * AimDir);
			FocallLocation = CamLocation + (AimDir * FocalDistance);
		}
		else if (AAIController* AIController = Cast<AAIController>(SourcePawnController))
		{
			CamLocation = SourcePawn->GetActorLocation() + FVector(0, 0, SourcePawn->BaseEyeHeight);
		}

		if (Source == ETPSAbilityTargetingSource::CameraTowardsFocus)
		{
			return FTransform(CamRotation, CamLocation);
		}
	}

	if ((Source == ETPSAbilityTargetingSource::WeaponForward) || (Source ==
		ETPSAbilityTargetingSource::WeaponTowardsFocus))
	{
		SourceLocation = GetWeaponTargetingSourceLocation();
	}
	else
	{
		SourceLocation = ActorLocation;
	}

	if (bFoundFocus == true && ((Source == ETPSAbilityTargetingSource::PawnTowardsFocus) || (Source ==
		ETPSAbilityTargetingSource::WeaponTowardsFocus)))
	{
		return FTransform((FocallLocation - SourceLocation).Rotation(), SourceLocation);
	}
	return FTransform(AimQuat, SourceLocation);
}

void UTPSGA_Attack::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle,
                                              FGameplayTag GameplayTag)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(ASC);

	if (const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FScopedPredictionWindow ScopedPrediction(ASC);

		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(
			MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(GameplayAbilityTargetDataHandle)));

		const bool bShouldNotifyServer = CurrentActorInfo->IsLocallyControlled() == true && CurrentActorInfo->
			IsNetAuthority() == false;
		if (bShouldNotifyServer == true)
		{
			ASC->CallServerSetReplicatedTargetData(CurrentSpecHandle,
			                                       CurrentActivationInfo.GetActivationPredictionKey(),
			                                       LocalTargetDataHandle, GameplayTag, ASC->ScopedPredictionKey);
		}

		const bool bIsTargetDataValid = true;
		bool bProjectileWeapon = false;

#if WITH_SERVER_CODE
		if (bProjectileWeapon == false)
		{
			if (AController* Controller = GetControllerFromActorInfo())
			{
				if (Controller->GetLocalRole() == ROLE_Authority)
				{
					// 무기 추가 시 무기 관련 코드 해제 필요
					// if (UTPSWeaponStateComponent* WeaponStateComponent = Controller->FindComponentByClass<UTPSWeaponStateComponent>())
					// {
					// 	TArray<uint8> HitReplaces;
					// 	uint8 IndexLimit = 255;
					// 	for (uint8 Index = 0; Index < LocalTargetDataHandle.Num() && Index < IndexLimit; ++Index )
					// 	{
					// 		if (FGameplayAbilityTargetData_SingleTargetHit* SingleTargetHit = static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(LocalTargetDataHandle.Get(Index)))
					// 		{
					// 			if (SingleTargetHit->bHitReplaced == true)
					// 			{
					// 				HitReplaces.Add(Index);
					// 			}
					// 		}
					// 	}
					// 	WeaponStateComponent->ClientConfirmTargetData(LocalTargetDataHandle.UniqueId, bIsTargetDataValid, HitReplaces);
					// }


					// WeaponStateComponent->ClientConfirmTargetData(LocalTargetDataHandle.UniqueId, bIsTargetDataValid, HitReplaces);
				}
			}
		}
#endif
		if (bIsTargetDataValid == true && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) ==
			true)
		{
			// 무기 추가 시 무기 관련 코드 해제 필요
			// UTPSRangedWeaponInstance* WeaponData = GetWeapontInstance();
			// check(WeaponData);
			// WeaponData->AddSpread();

			OnRangedWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon ability %s failed to commit (bIsTargetDataValid=%d)"), *GetPathName(),
			       bIsTargetDataValid ? 1 : 0);
			K2_EndAbility();
		}
	}
	ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
}


void UTPSGA_Attack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (ATPSCharacter* Character = Cast<ATPSCharacter>(CurrentActorInfo->AvatarActor.Get()))
	{
		Character->OnEndDelegate.Broadcast();
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPSGA_Attack::OnReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	check(CurrentActorInfo);

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);

	UAbilitySystemComponent* AC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(AC);

	AController* Controller = GetControllerFromActorInfo();
	check(Controller);
	
	if (EventTag == TPSGameplayTags::Event_Montage_EndAbility)
	{
		if (ATPSCharacter* Character = Cast<ATPSCharacter>(AvatarActor))
		{
			Character->OnEndDelegate.Broadcast();
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	else
	{
		AttackExecute();
	}
}

void UTPSGA_Attack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (ATPSCharacter* Character = Cast<ATPSCharacter>(CurrentActorInfo->AvatarActor.Get()))
	{
		Character->OnEndDelegate.Broadcast();
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPSGA_Attack::OnRangedWeaponTargetDataReady_Implementation(const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (ToTargetGEs.Num() > 0)
	{
		for (TSubclassOf<UGameplayEffect> GE : ToTargetGEs)
		{
			//UE_LOG(LogTemp, Log,TEXT("Test OnRangedWeaponTargetDataReady %s"),*GetNameSafe(GE));
			ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData, GE, 1);
		}
	}
}
