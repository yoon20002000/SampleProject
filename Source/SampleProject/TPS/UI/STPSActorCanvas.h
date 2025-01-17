// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


class UTPSIndicatorDescriptor;
/**
 * 
 */
class STPSActorCanvas : public SPanel
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

		bool GetIsDirty() const;
		void ClearDirtyFlag();
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

		mutable  uint8 bWasIndicatorClamped :1;
		mutable  uint8 bWasIndicatorClampedStatusChanged :1;

		friend class SActorCanvas;
	};
};
