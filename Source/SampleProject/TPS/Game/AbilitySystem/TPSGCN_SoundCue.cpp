// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AbilitySystem/TPSGCN_SoundCue.h"

#include "Character/TPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


UTPSGCN_SoundCue::UTPSGCN_SoundCue()
{
}

FVector UTPSGCN_SoundCue::GetSpawnLocation(const FHitResult& HitResult) const
{
	return HitResult.Location;
}

bool UTPSGCN_SoundCue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if (const FHitResult* HitResult = Parameters.EffectContext.GetHitResult())
	{
		if (ATPSCharacter* Character = Cast<ATPSCharacter>(HitResult->GetActor()))
		{
			UGameplayStatics::SpawnSoundAttached(SoundCue,Character->GetMesh(),TEXT("Light03"));	
		}	
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
