// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSHUDLayout.h"

#include "CommonUIExtensions.h"
#include "TPSCommonActivatableWidget.h"
#include "NativeGameplayTags.h"
#include "UITag.h"
#include"CommonUI/Public/Input/CommonUIInputTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TPSHUDLayout)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");

UTPSHUDLayout::UTPSHUDLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTPSHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE),
	                                          FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
}

void UTPSHUDLayout::HandleEscapeAction() const
{
	if (ensure(EscapeMenuClass.IsNull() == false))
	{
		UCommonUIExtensions::PushContentToLayer(TAG_UI_LAYER_MENU, EscapeMenuClass.LoadSynchronous());
	}
}
