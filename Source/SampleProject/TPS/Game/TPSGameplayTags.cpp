// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSGameplayTags.h"

namespace TPSGameplayTags
{
	/// Ability
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Action_WeaponFire, "Ability.Type.Action.WeaponFire", "Ability Weapon fire");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Action_WeaponSPFire, "Ability.Type.Action.WeaponSPFire", "Ability Weapon SP fire");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Action_Item_Acquire, "Ability.Type.Action.Item.Acquire", "Ability Type Action Item Acquire");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "Ability fail cooldown");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost", "Ability fail cost");
	
	
	/// Action Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Attack,"Action.Attack", "Character Attack Action");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Action_Jump,"Action.Jump", "Character Jump Action");

	/// Weapon
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_WeaponFireBlocked, "Ability.Weapon.NoFiring", "Weapon Shot Block Tag");
	
	/// Input Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (Mouse) Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (Stick) Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump Input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_Fire, "InputTag.Weapon.Fire", "Input Weapon Fire");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Interaction, "InputTag.Interaction", "Input Interaction");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_SPFire, "InputTag.Weapon.SPFire", "Input Weapon SPFire");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Item_Acquire, "InputTag.Item.Acquire", "Input Item Acquire");
	

	/// InitState
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "Init Spawn");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "Gameplay Ready");

	/// GameplayEvent
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event when fire on death");

	/// Status
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Crouching, "Status.Crouching", "Target is Crouching");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dying, "Status.Death.Dying", "Target is Dying");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Death, "Status.Death.Death", "Target is Death");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Healing, "Status.Healing", "Target is Healing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Item_Targeting, "Status.Item.Targeting", "Status Item Targeting");

	/// Movement
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

	/// GameplayEvent
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Damage, "Gameplay.Damage", "Gameplay Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_DamageImmunity, "Gameplay.DamageImmunity", "Gameplay Damage Immunity");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_DamageSelfDestruct, "Gameplay.DamageSelfDestruct", "Gameplay Damage Self Destruct");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_FellOutOfWorld, "Gameplay.FellOutOfWorld", "Gameplay Fell Out Of World");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Damage_Message, "Gameplay.Damage.Message", "Gameplay Damage Message");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Interaction, "Gameplay.Interaction", "Gameplay Interaction");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Gameplay_Heal, "Gameplay.Heal", "Gameplay Heal");
	
	

	/// Event
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_WeaponFire, "Event.Movement.WeaponFire", "Event Weapon fire");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_WeaponSPFire, "Event.Movement.WeaponSPFire", "Event Weapon SP fire");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_EndAbility, "Event.Montage.EndAbility", "Event Montage EndAbility");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_Fire, "Event.Montage.Fire", "Event Montage fire");

	/// Cooldown
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooldown_WeaponSPFire, "Cooldown.WeaponSPFire", "Cooldown Weapon SP Fire");
}

namespace TPSGGameplayEffectTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEffect_DamageType_Normal,"GameplayEffect.DamageType.Normal", "Damage Type Normal");
}
namespace TPSGameplayCueTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Character_DamageTaken,"GameplayCue.Character.DamageTaken", "Character Damaged Play Cue");
}

namespace TPSUILayerTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_LAYER_Menu,"UI.Layer.Menu", "Layer Menu");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_LAYER_Game,"UI.Layer.Game", "Layer Game");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_LAYER_GameMenu,"UI.Layer.GameMenu", "Layer GameMenu");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_LAYER_Modal,"UI.Layer.Modal", "Layer Modal");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_UI_ACTION_ESCAPE,"UI.Action.Escape", "UI Action Escape");
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
