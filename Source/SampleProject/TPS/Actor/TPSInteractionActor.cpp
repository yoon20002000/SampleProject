#include "Actor/TPSInteractionActor.h"

#include "Components/SphereComponent.h"
#include "Game/AbilitySystem/TPSAbilitySystemComponent.h"

// Sets default values
ATPSInteractionActor::ATPSInteractionActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetCollisionProfileName("InteractionObject");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetupAttachment(RootComponent);
	PrimaryActorTick.bCanEverTick = false;
	
}

FActiveGameplayEffectHandle ATPSInteractionActor::ApplyGE(UTPSAbilitySystemComponent* AbilitySystemComponent)
{
	check(AbilitySystemComponent);

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();

	const UGameplayEffect* Effect = EffectClass->GetDefaultObject<UGameplayEffect>();
	
	FActiveGameplayEffectHandle ApplyEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect,1,EffectContext);
	
	return ApplyEffectHandle;
}

// Called when the game starts or when spawned
void ATPSInteractionActor::BeginPlay()
{
	Super::BeginPlay();
}
