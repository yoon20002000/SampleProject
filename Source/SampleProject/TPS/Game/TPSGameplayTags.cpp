// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSGameplayTags.h"

namespace TPSGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Action_WeaponFire, "Ability.Type.Action.WeaponFire", "Ability Weapon fire");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Attack,"Action.Attack", "Character Attack Action");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Jump,"Action.Jump", "Character Jump Action");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (Mouse) Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (Stick) Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump Input");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "Init Spawn");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "Gameplay Ready");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event when fire on death");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Crouching, "Status.Crouching", "Target is Crouching");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dying, "Status.Death.Dying", "Target is Dying");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Death, "Status.Death.Death", "Target is Death");

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

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Damage, "Gameplay.Damage", "Gameplay Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_DamageImmunity, "Gameplay.DamageImmunity", "Gameplay Damage Immunity");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_DamageSelfDestruct, "Gameplay.DamageSelfDestruct", "Gameplay Damage Self Destruct");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_FellOutOfWorld, "Gameplay.FellOutOfWorld", "Gameplay Fell Out Of World");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Damage_Message, "Gameplay.Damage.Message", "Gameplay Damage Message");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_WeaponFire, "Event.Movement.WeaponFire", "Event Weapon fire");
}

namespace TPSGGameplayEffectTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEffect_DamageType_Normal,"GameplayEffect.DamageType.Normal", "Damage Type Normal");
}
namespace TPSGameplayCueTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_DamageTaken,"GameplayCue.Character.DamageTaken", "Character Damaged Play Cue");
}

FGameplayTag FTPSGameplayTagHelper::FindTagByString(const FString& TagString, bool bMatchPartialsString)
{
	const UGameplayTagsManager& GTM = UGameplayTagsManager::Get();

	FGameplayTag Tag = GTM.RequestGameplayTag(FName(*TagString));
	if (Tag.IsValid() == false && bMatchPartialsString == true)
	{
		FGameplayTagContainer TagContainer;
		GTM.RequestAllGameplayTags(TagContainer, true);

		for (const FGameplayTag TagElem : TagContainer)
		{
			if (TagElem.ToString().Contains(TagString) == true)
			{
				UE_LOG(LogTemp, Warning, TEXT("Applied Partial !! InputTagString : %s / Tag : %s"), *TagString, *TagElem.ToString());
				Tag = TagElem;
				break;
			}
		}
	}
	return Tag;
}
