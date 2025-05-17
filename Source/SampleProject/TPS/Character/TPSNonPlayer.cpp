#include "Character/TPSNonPlayer.h"

#include "TPSPlayer.h"
#include "Components/TPSThreatSenseComponent.h"

ATPSNonPlayer::ATPSNonPlayer()
{
	ThreatSenseComponent = CreateDefaultSubobject<UTPSThreatSenseComponent>(TEXT("ThreatSense Component"));
}

void ATPSNonPlayer::OnHealthChanged(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue,
                                    AActor* InstigatorActor)
{
	Super::OnHealthChanged(HealthComponent, OldValue, NewValue, InstigatorActor);

	float Damage = OldValue - NewValue;
	ThreatSenseComponent->ReportThreat(Cast<ATPSPlayer>(InstigatorActor), Damage);
}

void ATPSNonPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ThreatSenseComponent->ThreatDecay(DeltaSeconds);
}
