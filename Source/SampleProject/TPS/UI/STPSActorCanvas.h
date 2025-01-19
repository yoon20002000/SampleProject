// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Blueprint/UserWidgetPool.h"
#include "AsyncMixin.h"

class UTPSIndicatorManagerComponent;
struct FUserWidgetPool;
class UTPSIndicatorDescriptor;
/**
 * 
 */
class STPSActorCanvas : public SPanel, public FAsyncMixin
{
public:
	class FSlot : public TSlotBase<FSlot>
	{
	public:
		FSlot(UTPSIndicatorDescriptor* InIndicator);

		SLATE_SLOT_BEGIN_ARGS(FSlot, TSlotBase<FSlot>)
		SLATE_SLOT_END_ARGS()
		using TSlotBase<FSlot>::Construct;

		bool GetIsIndicatorVisible() const;
		void SetIsIndicatorVisible(bool bVisible);

		FVector2D GetScreenPosition() const;
		void SetScreenPosition(FVector2D InScreenPosition);

		double GetDepth() const;
		void SetDepth(double InDepth);

		int32 GetPriority() const;
		void SetPriority(int32 InPriority);

		bool GetInFrontOfCamera() const;
		void SetInFrontOfCamera(bool InFrontOfCamera);

		bool HasValidScreenPosition() const;
		void SetHasValidScreenPosition(bool InHasValidScreenPosition);

		bool IsDirty() const;
		void ClearDirtyFlag();

		bool IsIndicatorClamped() const;
		void SetIndicatorClamped(bool InClamped);

		bool IsIndicatorClampedStatusChanged() const;
		void ClearIndicatorClampedStatusChanged();
		
	private:
		void RefreshVisibility() const;
		
	private:

		UTPSIndicatorDescriptor* Indicator;
		FVector2D ScreenPosition;
		double Depth;
		int32 Priority;

		uint8 bIsIndicatorVisible : 1;
		uint8 bInFrontOfCamera : 1;
		uint8 bHasValidScreenPosition :1;
		uint8 bDirty : 1;

		mutable  uint8 bIsIndicatorClamped :1;
		mutable  uint8 bIsIndicatorClampedStatusChanged :1;

		friend class STPSActorCanvas;
	};
	class FArrowSlot : public TSlotBase<FArrowSlot>
	{
	};

	SLATE_BEGIN_ARGS(STPSActorCanvas)
	{
		_Visibility = EVisibility::HitTestInvisible;
	}
		SLATE_SLOT_ARGUMENT(STPSActorCanvas::FSlot, Slots)
	SLATE_END_ARGS()

public:
	STPSActorCanvas();

	void Construct(const FArguments& InArgs, const FLocalPlayerContext& InLocalPlayerContext, const FSlateBrush* InActorCanvasArrowBrush);

private:
	void OnIndicatorAdded(UTPSIndicatorDescriptor* IndicatorDescriptor);
	void OnIndicatorRemoved(UTPSIndicatorDescriptor* IndicatorDescriptor);

	void AddIndicatorForEntry(UTPSIndicatorDescriptor* IndicatorDescriptor);
	void RemoveIndicatorForEntry(UTPSIndicatorDescriptor* IndicatorDescriptor);

	using FScopedWidgetSlotArguments = TPanelChildren<FSlot>::FScopedWidgetSlotArguments;
	FScopedWidgetSlotArguments AddActorSlot(UTPSIndicatorDescriptor* Indicator);
	// 제거된 Slot Index Return, sentinel num : -1
	int32 RemoveActorSlot(const TSharedRef<SWidget>& SlotWidget);

	void SetShowAnyIndicators(bool ShowAnyIndicators);
	EActiveTimerReturnType UpdateCanvas(double InCurrentTime, float InDeltaTime);
	void UpdateActiveTimer();
private:
	// 모든 Indicator 정보
	TArray<TObjectPtr<UTPSIndicatorDescriptor>> AllIndicators;
	TArray<UTPSIndicatorDescriptor*> InactiveIndicators;
	
	FLocalPlayerContext LocalPlayerContext;
	TWeakObjectPtr<UTPSIndicatorManagerComponent> IndicatorComp;

	
	
	
	// 캔버스 내 모든 Slot
	TPanelChildren<FSlot> CanvasChildren;
	mutable TPanelChildren<FArrowSlot> ArrowsChildren;
	FCombinedChildren AllChildren;

	FUserWidgetPool IndicatorPool;
	
	const FSlateBrush* ActorCanvasArrowBrush = nullptr;

	bool bShowAnyIndicators = false;

	mutable TOptional<FGeometry> OptionalPaintGeometry;

	TSharedPtr<FActiveTimerHandle> TickHandle;
};
