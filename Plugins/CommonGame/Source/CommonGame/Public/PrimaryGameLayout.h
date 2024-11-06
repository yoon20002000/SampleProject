// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CommonActivatableWidget.h"
#include "CommonUIExtensions.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "PrimaryGameLayout.generated.h"

struct FGameplayTag;
struct FStreamableHandle;
class APlayerController;
class UClass;
class UCommonActivatableWidgetContainerBase;
class ULocalPlayer;
class UObject;
struct FFrame;

/**
 * UI load state operation
 */
enum class EAsyncWidgetLayerState:uint8
{
	Canceled,
	Initialize,
	AfterPush
};

/**
 * 기본 게임 UI Layout
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class COMMONGAME_API UYPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()


public:
	static UYPrimaryGameLayout* GetPrimaryGameLayoutForPrimaryPlayer(const UObject* WorldContextObject);
	static UYPrimaryGameLayout* GetPrimaryGameLayout(APlayerController* PlayerController);
	static UYPrimaryGameLayout* GetPrimaryGameLayout(ULocalPlayer* LocalPlayer);

public:
	UYPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

	void SetIsDormant(bool bInDormant);
	bool IsDormant() const { return bIsDormant; }

public:
	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete,
	                                                          TSoftClassPtr<UCommonActivatableWidget>
	                                                          ActivatableWidgetClass)
	{
		return PushWidgetToLayerStackAsync<ActivatableWidgetT>(LayerName, bSuspendInputUntilComplete,
		                                                       ActivatableWidgetClass,
		                                                       [](EAsyncWidgetLayerState, ActivatableWidgetT* )
		                                                       {
		                                                       });
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete,
	                                                          TSoftClassPtr<UCommonActivatableWidget>
	                                                          ActivatableWidgetClass,
	                                                          TFunction<void(
		                                                          EAsyncWidgetLayerState,
		                                                          ActivatableWidgetT*)> StateFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived,
		              "Only CommonActivatableWidgets can be used here");
		static FName NAME_PushingWidgetToLayer("PushingWidgetToLayer");
		const FName SuspendInputToken = bSuspendInputUntilComplete == true
			                                ? UYCommonUIExtensions::SuspendInputForPlayer(
				                                GetOwningPlayer(), NAME_PushingWidgetToLayer)
			                                : NAME_None;
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(
			ActivatableWidgetClass.ToSoftObjectPath(),
			FStreamableDelegate::CreateWeakLambda(this,
			                                      [this, LayerName, ActivatableWidgetClass, StateFunc, SuspendInputToken
			                                      ]()
			                                      {
				                                      UYCommonUIExtensions::ResumeInputForPlayer(
					                                      GetOwningPlayer(), SuspendInputToken);
				                                      ActivatableWidgetT* Widget = PushWidgetToLayerStack<
					                                      ActivatableWidgetT>(
					                                      LayerName, ActivatableWidgetClass.Get(),
					                                      [StateFunc](ActivatableWidgetT& WidgetToInit)
					                                      {
						                                      StateFunc(EAsyncWidgetLayerState::Initialize,
						                                                &WidgetToInit);
					                                      }
				                                      );
				                                      StateFunc(EAsyncWidgetLayerState::Initialize, Widget);
			                                      })
		);

		StreamingHandle->BindCancelDelegate(FStreamableDelegate::CreateWeakLambda(this,
				[this, StateFunc, SuspendInputToken]()
				{
					UYCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);
					StateFunc(EAsyncWidgetLayerState::Canceled, nullptr);
				})
		);
		return StreamingHandle;
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT&)
		{
		});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass,
	                                           TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived,
		              "Only CommonActivatableWidgets can be used here");

		if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
		{
			return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);
		}
		return nullptr;
	}

	void FindAndRemoveWidgetFromLayer(UCommonActivatableWidget* ActivatableWidget);
	
	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerName);

protected:
	UFUNCTION(BlueprintCallable, Category="Layer")
	void RegisterLayer(UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);
	virtual void OnIsDormantChanged();
	void OnWidgetStackTransitioning(UCommonActivatableWidgetContainerBase* Widget, bool bIsTransitioning);

private:
	bool bIsDormant = false;

	TArray<FName> SuspendInputTokens;
	
	UPROPERTY(Transient, meta = (Categories = "UI.Layrer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
