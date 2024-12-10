// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSGameplayTags.h"

namespace TPSGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (Mouse) Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (Stick) Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch Input");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "Init Spawn");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "Gameplay Ready");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event when fire on death");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Crouching, "Status.Crouching", "Target is Crouching");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target is Death");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking, "Movement.Mode.Walking",
	                               "Default Character Movement Walking");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Falling, "Movement.Mode.Falling",
	                               "Default Character Movement Falling");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Flying, "Movement.Mode.Flying", "Default Character Movement Flying");

	const TMap<uint8, FGameplayTag> MovementModeTagMap =
	{
		{MOVE_Walking, Movement_Mode_Walking},
		{MOVE_Falling, Movement_Mode_Falling},
		{MOVE_Flying, Movement_Mode_Flying},
	};
	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& GTM = UGameplayTagsManager::Get();
		FGameplayTag Tag = GTM.RequestGameplayTag(FName(*TagString));
		if (Tag.IsValid() == false && bMatchPartialString == true)
		{
			FGameplayTagContainer AllTags;
			GTM.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				UE_LOG(LogTemp, Warning, TEXT("Partial String Matched. Applied Tag : %s"), *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
		return Tag;
	}
}
