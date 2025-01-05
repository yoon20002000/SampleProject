// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSHUDLayout.h"

#include "CommonUIExtensions.h"
#include "TPSCommonActivatableWidget.h"
#include "NativeGameplayTags.h"
#include "TPSHelper.h"
#include "UITag.h"
#include"CommonUI/Public/Input/CommonUIInputTypes.h"
#include "Game/TPSGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TPSHUDLayout)

UTPSHUDLayout::UTPSHUDLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UTPSHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TPSUILayerTags::TAG_UI_ACTION_ESCAPE),
	                                          FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
}

void UTPSHUDLayout::HandleEscapeAction() const
{
	if (ensure(EscapeMenuClass.IsNull() == false))
	{
		UCommonUIExtensions::PushContentToLayer(TPSUILayerTags::TAG_UI_LAYER_Menu, EscapeMenuClass.LoadSynchronous());
		APlayerController* PC = TPSHelper::GetPlayerController(GetWorld());
		PC->SetInputMode(FInputModeUIOnly());
	}
}
