#include "Components/TPSThreatSenseComponent.h"

#include "Character/TPSAIController.h"
#include "Character/TPSPlayer.h"

UTPSThreatSenseComponent::UTPSThreatSenseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ThreatDecayAmount = 10.0f;
	ThreatThreshold = 1;
	AlertThreshold = ThreatThreshold * .5f;
}

void UTPSThreatSenseComponent::ReportThreat(ATPSPlayer* InSourceActor, float InAmount)
{
	if (InSourceActor == nullptr || InAmount <= 0)
	{
		return;
	}

	float& CurrentThreat = ThreatMap.FindOrAdd(InSourceActor);
	CurrentThreat += InAmount;
}

void UTPSThreatSenseComponent::ThreatDecay(float InDeltaTime)
{
	for (auto Iter = ThreatMap.CreateIterator(); Iter; ++Iter)
	{
		if (IsValid(Iter.Key().Get()) == false)
		{
			Iter.RemoveCurrent();
			continue;
		}

		Iter.Value() -= ThreatDecayAmount * InDeltaTime;

		if (Iter.Value() <= 0 || Iter.Key()->IsAlive() == false)
		{
			Iter.RemoveCurrent();
			continue;
		}
	}
}

EThreatLevel UTPSThreatSenseComponent::GetCurrentThreatLevel() const
{
	float TotalThreat = 0;
	for (const auto& Elem : ThreatMap)
	{
		TotalThreat += Elem.Value;
	}

	if (TotalThreat >= ThreatThreshold)
	{
		return EThreatLevel::Enraged;
	}
	else if (TotalThreat >= AlertThreshold)
	{
		return EThreatLevel::Alert;
	}
	else
	{
		return EThreatLevel::Calm;
	}
}

AActor* UTPSThreatSenseComponent::GetHighestActor() const
{
	ATPSCharacter* HighestThreadActor = nullptr;
	float HighestThreat = 0.f;
	float TotalThreat = 0.f;
	for (auto& Iter : ThreatMap)
	{
		if (IsValid(Iter.Key.Get()) == false)
		{
			continue;
		}

		if (Iter.Value <= 0)
		{
			continue;
		}
		
		if (Iter.Value > HighestThreat && Iter.Key->IsAlive())
		{
			HighestThreat = Iter.Value;
			TotalThreat += Iter.Value;
			HighestThreadActor = Iter.Key.Get();
		}
	}
	if (TotalThreat >= ThreatThreshold)
	{
		return HighestThreadActor;
	}
	else
	{
		return nullptr;
	}
}

void UTPSThreatSenseComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* Pawn = Cast<APawn>(GetOwner()); Pawn != nullptr)
	{
		AIController = Cast<ATPSAIController>(Pawn->GetController());
	}
}
