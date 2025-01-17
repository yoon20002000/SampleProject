// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STPSActorCanvas.h"

#include "MathUtil.h"

// 작업 후 이전 문제없는지 확인 후 이전 할 것
class STPSActorCanvasArrowWidget : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(STPSActorCanvasArrowWidget)
	{
		
	}
	SLATE_END_ARGS()
	STPSActorCanvasArrowWidget():Rotation(0.0f), Arrow(nullptr)
	{
		
	}

	void Construct(const FArguments& InArgs, const FSlateBrush* ActorCanvasArrowBrush)
	{
		Arrow = ActorCanvasArrowBrush;
		SetCanTick(false);
	}

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& ClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerID, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override
	{
		int32 MaxLayerID = LayerID;
		if (Arrow != nullptr)
		{
			const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
			const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
			const FColor FinalColorAndOpacity = (InWidgetStyle.GetColorAndOpacityTint() * Arrow->GetTint(InWidgetStyle)).ToFColor(true);

			FSlateDrawElement::MakeRotatedBox(
				OutDrawElements,
				MaxLayerID++,AllottedGeometry.ToPaintGeometry(Arrow->ImageSize,FSlateLayoutTransform()),
				Arrow,
				DrawEffects,
				FMath::DegreesToRadians(GetRotation()),
				TOptional<FVector2D>(),
				FSlateDrawElement::RelativeToElement,
				FinalColorAndOpacity
				);
		}
		
		return MaxLayerID;
	}

	FORCEINLINE void SetRotation(float InRotation)
	{
		Rotation=FMath::Fmod(InRotation,360.0f);
	}
	FORCEINLINE float GetRotation() const
	{
		return Rotation;
	}

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override
	{
		if (Arrow != nullptr)
		{
			return Arrow->ImageSize;
		}
		else
		{
			return FVector2D::ZeroVector;
		}
	}
	
private:
	float Rotation;
	const FSlateBrush* Arrow;
};
bool STPSActorCanvas::FSlot::GetIsIndicatorVisible() const
{
	return bIsIndicatorVisible;
}

void STPSActorCanvas::FSlot::SetIsIndicatorVisible(bool bVisible)
{
	if (bIsIndicatorVisible != bVisible)
	{
		bIsIndicatorVisible = bVisible;
		bDirty = true;
	}
	RefreshVisibility();
}
FVector2D STPSActorCanvas::FSlot::GetScreenPosition() const
{
	return ScreenPosition;
}

void STPSActorCanvas::FSlot::SetScreenPosition(FVector2D InScreenPosition)
{
	if (ScreenPosition != InScreenPosition)
	{
		ScreenPosition = InScreenPosition;
		bDirty = true;
	}
}
double STPSActorCanvas::FSlot::GetDepth() const
{
	return Depth;
}

void STPSActorCanvas::FSlot::SetDepth(double InDepth)
{
	if (Depth != InDepth)
	{
		Depth = InDepth;
		bDirty = true;
	}
}
int32 STPSActorCanvas::FSlot::GetPriority() const
{
	return Priority;
}

void STPSActorCanvas::FSlot::SetPriority(int32 InPriority)
{
	if (Priority != InPriority)
	{
		Priority = InPriority;
		bDirty = true;
	}
}
bool STPSActorCanvas::FSlot::GetInFrontOfCamera() const
{
	return bInFrontOfCamera;
}

void STPSActorCanvas::FSlot::SetInFrontOfCamera(bool InFrontOfCamera)
{
	if (bInFrontOfCamera != InFrontOfCamera)
	{
		bInFrontOfCamera = InFrontOfCamera;
		bDirty = true;
	}
	RefreshVisibility();
}
bool STPSActorCanvas::FSlot::HasValidScreenPosition() const
{
	return bHasValidScreenPosition;
}

void STPSActorCanvas::FSlot::SetHasValidScreenPosition(bool InHasValidScreenPosition)
{
	if (bHasValidScreenPosition != InHasValidScreenPosition)
	{
		bHasValidScreenPosition = InHasValidScreenPosition;
		bDirty = true;
	}
	RefreshVisibility();
}
bool STPSActorCanvas::FSlot::IsDirty() const
{
	return bDirty;
}
void STPSActorCanvas::FSlot::ClearDirtyFlag()
{
	bDirty = false;
}
bool STPSActorCanvas::FSlot::IsIndicatorClamped() const
{
	return bIsIndicatorVisible;
}

void STPSActorCanvas::FSlot::SetIndicatorClamped(bool InClamped)
{
	if (bIsIndicatorClamped != InClamped)
	{
		bIsIndicatorClamped = InClamped;
		bIsIndicatorClampedStatusChanged = true;
	}
}

bool STPSActorCanvas::FSlot::IsIndicatorClampedStatusChanged() const
{
	return bIsIndicatorClampedStatusChanged;
}

void STPSActorCanvas::FSlot::ClearIndicatorClampedStatusChanged()
{
	bIsIndicatorClampedStatusChanged = false;
}

void STPSActorCanvas::FSlot::RefreshVisibility() const
{
	const bool bIsVisible = bIsIndicatorVisible && bHasValidScreenPosition;
	GetWidget()->SetVisibility(bIsVisible == true ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed);
}

STPSActorCanvas::STPSActorCanvas() : CanvasChildren(this), ArrowsChildren(this), AllChildren(this)
{
	AllChildren.AddChildren(CanvasChildren);
	AllChildren.AddChildren(ArrowsChildren);
}

void STPSActorCanvas::Construct(const FArguments& InArgs, const FLocalPlayerContext& InLocalPlayerContext,
	const FSlateBrush* InActorCanvasArrowBrush)
{
	LocalPlayerContext = InLocalPlayerContext;
	ActorCanvasArrowBrush = InActorCanvasArrowBrush;

	IndicatorPool.SetWorld(LocalPlayerContext.GetWorld());

	SetCanTick(false);
	SetVisibility(EVisibility::SelfHitTestInvisible);

	for (int32 indx = 0; indx < AllChildren.Num(); ++indx)
	{
		TSharedRef<STPSActorCanvasArrowWidget> ArrowWidget = SNew(STPSActorCanvasArrowWidget, ActorCanvasArrowBrush);
		ArrowWidget->SetVisibility(EVisibility::Collapsed);

		ArrowsChildren.AddSlot(MoveTemp(
			FArrowSlot::FSlotArguments(MakeUnique<FArrowSlot>())
			[
				ArrowWidget
			]
		));
	}

	UpdateActiveTimer();
}

void STPSActorCanvas::UpdateActiveTimer()
{
}
