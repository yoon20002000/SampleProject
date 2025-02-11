#include "Actor/TPSApplyGEActor.h"

#include "Components/SphereComponent.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"

// Sets default values
ATPSApplyGEActor::ATPSApplyGEActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetCollisionProfileName("InteractionObject");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetupAttachment(RootComponent);
	PrimaryActorTick.bCanEverTick = false;
	
}

FActiveGameplayEffectHandle ATPSApplyGEActor::ApplyGE(UTPSAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();

	const UGameplayEffect* Effect = EffectClass->GetDefaultObject<UGameplayEffect>();
	
	FActiveGameplayEffectHandle ApplyEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect,1,EffectContext);
	
	return ApplyEffectHandle;
}

FString ATPSApplyGEActor::GetInteractionDescription() const
{
	return InteractionDescription;
}

void ATPSApplyGEActor::Interaction(AActor* InteractionInstigatorActor)
{
	if (UTPSAbilitySystemComponent* ASC = InteractionInstigatorActor->GetComponentByClass<UTPSAbilitySystemComponent>())
	{
		ApplyGE(ASC);	
	}
}
