#include "Actor/TPSAIStart.h"

#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/CapsuleComponent.h"

ATPSAIStart::ATPSAIStart(const FObjectInitializer& ObjectInitializer)
{
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 92.0f);
	GetCapsuleComponent()->SetShouldUpdatePhysicsVolume(false);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	
	if (!IsRunningCommandlet())
	{
		// Structure to hold one-time initialization
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> AIStartTextureObject;
			FName ID_AIStart;
			FText NAME_AIStart;
			FName ID_Navigation;
			FText NAME_Navigation;
			FConstructorStatics()
				: AIStartTextureObject(TEXT("/Game/Resources/Textures/S_AIStart.S_AIStart"))
				, ID_AIStart(TEXT("AIStart"))
				, NAME_AIStart(NSLOCTEXT("SpriteCategory", "AIStart", "AI Start"))
				, ID_Navigation(TEXT("Navigation"))
				, NAME_Navigation(NSLOCTEXT("SpriteCategory", "Navigation", "Navigation"))
			{
			}
		};
		static FConstructorStatics ConstructorStatics;

		if (GetGoodSprite())
		{
			GetGoodSprite()->Sprite = ConstructorStatics.AIStartTextureObject.Get();
			GetGoodSprite()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
			GetGoodSprite()->SpriteInfo.Category = ConstructorStatics.ID_AIStart;
			GetGoodSprite()->SpriteInfo.DisplayName = ConstructorStatics.NAME_AIStart;
		}
		if (GetBadSprite())
		{
			GetBadSprite()->SetVisibility(false);
		}

		if (ArrowComponent)
		{
			ArrowComponent->ArrowColor = FColor(200, 200, 255);

			ArrowComponent->ArrowSize = 1.0f;
			ArrowComponent->bTreatAsASprite = true;
			ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Navigation;
			ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Navigation;
			ArrowComponent->SetupAttachment(GetCapsuleComponent());
			ArrowComponent->bIsScreenSizeScaled = true;
		}
	}

	bIsSpatiallyLoaded = false;
#endif // WITH_EDITORONLY_DATA
}
#if WITH_EDITORONLY_DATA
/** Returns ArrowComponent subobject **/
UArrowComponent* ATPSAIStart::GetArrowComponent() const { return ArrowComponent; }
#endif
