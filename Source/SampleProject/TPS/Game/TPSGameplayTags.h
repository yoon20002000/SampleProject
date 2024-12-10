// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NativeGameplayTags.h"

namespace TPSGameplayTags
{
	SAMPLEPROJECT_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	
	/// Input Tags
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);

	/// InitState
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	/// GameplayEvent
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);

	/// Status
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);

	/// Movement
	SAMPLEPROJECT_API extern const TMap<uint8, FGameplayTag> MovementModeTags;

	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	SAMPLEPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);
}
