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
		FSlot(UTPSIndicatorDescriptor* InIndicator): TSlotBase<FSlot>(),
		Indicator(InIndicator)
		{
			
		}
		
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
