// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STPSActorCanvas.h"
#include "TPSIndicatorDescriptor.h"
#include "TPSIndicatorWidgetInterface.h"
#include "Components/TPSIndicatorManagerComponent.h"

namespace EArrowDirection
{
	enum Type
	{
		Left,
		Top,
		Right,
		Bottom,
		Max
	};
}

const float ArrowRotations[EArrowDirection::Max] =
{
	270.0f,
	0.0f,
	90.0f,
	180.0f
};

const FVector2D ArrowOffsets[EArrowDirection::Max] =
{
	FVector2D(-1.0f, 0.0f),
	FVector2D(0.0f, -1.0f),
	FVector2D(1.0f, 0.0f),
	FVector2D(0.0f, 1.0f),
};


// 작업 후 이전 문제없는지 확인 후 이전 할 것
class STPSActorCanvasArrowWidget : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(STPSActorCanvasArrowWidget)
		{
		}

	SLATE_END_ARGS()

	STPSActorCanvasArrowWidget(): Rotation(0.0f), Arrow(nullptr)
	{
	}

	void Construct(const FArguments& InArgs, const FSlateBrush* ActorCanvasArrowBrush)
	{
		Arrow = ActorCanvasArrowBrush;
		SetCanTick(false);
	}

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& ClippingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerID, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override
	{
		int32 MaxLayerID = LayerID;
		if (Arrow != nullptr)
		{
			const bool bIsEnabled = ShouldBeEnabled(bParentEnabled);
			const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
			const FColor FinalColorAndOpacity = (InWidgetStyle.GetColorAndOpacityTint() * Arrow->GetTint(InWidgetStyle))
				.ToFColor(true);

			FSlateDrawElement::MakeRotatedBox(
				OutDrawElements,
				MaxLayerID++, AllottedGeometry.ToPaintGeometry(Arrow->ImageSize, FSlateLayoutTransform()),
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
		Rotation = FMath::Fmod(InRotation, 360.0f);
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

void STPSActorCanvas::FSlot::SetIndicatorClamped(bool InClamped) const
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

STPSActorCanvas::STPSActorCanvas() : CanvasChildren(this), ArrowChildren(this), AllChildren(this)
{
	AllChildren.AddChildren(CanvasChildren);
	AllChildren.AddChildren(ArrowChildren);
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

		ArrowChildren.AddSlot(MoveTemp(
			FArrowSlot::FSlotArguments(MakeUnique<FArrowSlot>())
			[
				ArrowWidget
			]
		));
	}

	UpdateActiveTimer();
}

void STPSActorCanvas::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	NextArrowIndex = 0;

	if (bShowAnyIndicators == true)
	{
		const FVector2D ArrowWidgetSize = ActorCanvasArrowBrush->GetImageSize();
		const FIntPoint FixedPadding = FIntPoint(10.0f, 10.0f) + FIntPoint(ArrowWidgetSize.X, ArrowWidgetSize.Y);
		const FVector Center = FVector(AllottedGeometry.Size * 0.5f, 0.0f);

		TArray<const STPSActorCanvas::FSlot*> SortedSlots;
		for (int32 ChildIndex = 0; ChildIndex < CanvasChildren.Num(); ++ChildIndex)
		{
			SortedSlots.Add(&CanvasChildren[ChildIndex]);
		}

		SortedSlots.StableSort([](const STPSActorCanvas::FSlot& X, const STPSActorCanvas::FSlot& Y)
		{
			if (X.GetPriority() == Y.GetPriority())
			{
				return X.GetDepth() < Y.GetDepth();
			}
			else
			{
				return X.GetPriority() < Y.GetPriority();
			}
		});

		for (int32 ChildIndex = 0; ChildIndex < SortedSlots.Num(); ++ChildIndex)
		{
			const FSlot& CurChild = *SortedSlots[ChildIndex];
			const UTPSIndicatorDescriptor* Indicator = CurChild.Indicator;

			if (ArrangedChildren.Accepts(CurChild.GetWidget()->GetVisibility()) == false)
			{
				CurChild.SetIndicatorClamped(false);
				continue;
			}
			FVector2D ScreenPosition = CurChild.GetScreenPosition();
			const bool bInFrontOfCamera = CurChild.GetInFrontOfCamera();

			const bool bShouldClamp = Indicator->GetClampToScreen();

			FVector2D SlotSize, SlotOffset, SlotPaddingMin, SlotPaddingMax;
			GetOffsetAndSize(Indicator, OUT SlotSize, OUT SlotOffset, OUT SlotPaddingMin, OUT SlotPaddingMax);

			bool bIsIndicatorClamped = false;

			if (bShouldClamp == true)
			{
				EArrowDirection::Type ClampDir = EArrowDirection::Max;

				const FIntPoint RectMin = FIntPoint(SlotPaddingMin.X, SlotPaddingMin.Y) + FixedPadding;
				const FIntPoint RectMax = FIntPoint(AllottedGeometry.Size.X - SlotPaddingMax.X,
				                                    AllottedGeometry.Size.Y - SlotPaddingMax.Y) - FixedPadding;
				const FIntRect ClampRect(RectMin, RectMax);

				if (ClampRect.Contains(FIntPoint(ScreenPosition.X, ScreenPosition.Y)) == false)
				{
					const FPlane Planes[] =
					{
						FPlane(FVector(1.0f, 0.0f, 0.0f), ClampRect.Min.X), // L
						FPlane(FVector(0.0f, 1.0f, 0.0f), ClampRect.Min.Y), // T
						FPlane(FVector(-1.0f, 0.0f, 0.0f), ClampRect.Max.X), // R
						FPlane(FVector(0.0f, -1.0f, 0.0f), ClampRect.Max.Y) // B
					};

					for (int32 Index = 0; Index < EArrowDirection::Max; ++Index)
					{
						FVector NewPoint;
						if (FMath::SegmentPlaneIntersection(Center, FVector(ScreenPosition, 0.0f), Planes[Index],
						                                    NewPoint))
						{
							ClampDir = static_cast<EArrowDirection::Type>(Index);
							ScreenPosition = FVector2D(NewPoint);
						}
					}
				}
				else if (bInFrontOfCamera == false)
				{
					const float ScreenXNorm = ScreenPosition.X / (RectMax.X - RectMin.X);
					const float ScreenYNorm = ScreenPosition.Y / (RectMax.Y - RectMin.Y);

					if (ScreenXNorm < ScreenYNorm)
					{
						if (ScreenXNorm < (-ScreenYNorm + 1.0f))
						{
							ClampDir = EArrowDirection::Left;
							ScreenPosition.X = ClampRect.Min.X;
						}
						else
						{
							ClampDir = EArrowDirection::Bottom;
							ScreenPosition.Y = ClampRect.Max.Y;
						}
					}
					else
					{
						if (ScreenXNorm < (-ScreenYNorm + 1.0f))
						{
							ClampDir = EArrowDirection::Top;
							ScreenPosition.Y = ClampRect.Min.Y;
						}
						else
						{
							ClampDir = EArrowDirection::Right;
							ScreenPosition.X = ClampRect.Max.X;
						}
					}
				}

				bIsIndicatorClamped = ClampDir != EArrowDirection::Max;

				if (Indicator->GetShowClampToScreenArrow() &&
					bIsIndicatorClamped == true &&
					ArrowChildren.IsValidIndex(NextArrowIndex) == true)
				{
					const FVector2D ArrowOffsetDirection = ArrowOffsets[ClampDir];
					const float ArrowRotation = ArrowRotations[ClampDir];

					TSharedRef<STPSActorCanvasArrowWidget> ArrowWidgetToUse = StaticCastSharedRef<
						STPSActorCanvasArrowWidget>(ArrowChildren.GetChildAt(NextArrowIndex));
					++NextArrowIndex;

					ArrowWidgetToUse->SetRotation(ArrowRotation);

					const FVector2D OffsetMagnitude = (SlotSize + ArrowWidgetSize) * 0.5f;

					const FVector2D ArrowCenteringOffset = -(ArrowWidgetSize * 0.5f);

					FVector2D ArrowAlignmentOffset = FVector2D::ZeroVector;
					if (Indicator->VAlignment == VAlign_Top)
					{
						ArrowAlignmentOffset = SlotSize * FVector2D(0.0f, 0.5f);
					}
					else if (Indicator->VAlignment == VAlign_Bottom)
					{
						ArrowAlignmentOffset = SlotSize * FVector2D(0.0f, -0.5f);
					}

					const FVector2D WidgetOffset = (OffsetMagnitude * ArrowOffsetDirection);
					const FVector2D FinalOffset = (WidgetOffset + ArrowAlignmentOffset + ArrowCenteringOffset);
					const FVector2D FinalPosition = (ScreenPosition + FinalOffset);

					ArrowWidgetToUse->SetVisibility(EVisibility::HitTestInvisible);

					ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ArrowWidgetToUse,FinalPosition,ArrowWidgetSize,1.f));
				}
			}
			CurChild.SetIndicatorClamped(bIsIndicatorClamped);

			ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(CurChild.GetWidget(), ScreenPosition + SlotOffset, SlotSize, 1.f));
		}
	}
	if (NextArrowIndex < ArrowIndexLastUpdate)
	{
		for (int32 ArrowRemovedIndex = NextArrowIndex; ArrowRemovedIndex < ArrowIndexLastUpdate; ++ArrowRemovedIndex)
		{
			ArrowChildren.GetChildAt(ArrowRemovedIndex)->SetVisibility(EVisibility::Visible);->SetVisibility(EVisibility::Collapsed);
		}
	}
	ArrowIndexLastUpdate = NextArrowIndex;
}

FVector2D STPSActorCanvas::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D::ZeroVector;
}

FChildren* STPSActorCanvas::GetChildren()
{
	return &AllChildren;
}

int32 STPSActorCanvas::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                               const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                               const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return SPanel::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                       bParentEnabled);
}

void STPSActorCanvas::OnIndicatorAdded(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	AllIndicators.Add(IndicatorDescriptor);
	InactiveIndicators.Add(IndicatorDescriptor);

	AddIndicatorForEntry(IndicatorDescriptor);
}

void STPSActorCanvas::OnIndicatorRemoved(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	RemoveIndicatorForEntry(IndicatorDescriptor);

	AllIndicators.Remove(IndicatorDescriptor);
	InactiveIndicators.Remove(IndicatorDescriptor);
}

void STPSActorCanvas::AddIndicatorForEntry(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	TSoftClassPtr<UUserWidget> IndicatorWidgetClass = IndicatorDescriptor->GetIndicatorWidgetClass();

	if (IndicatorWidgetClass.IsNull() == false)
	{
		TWeakObjectPtr<UTPSIndicatorDescriptor> IndicatorPtr(IndicatorDescriptor);
		AsyncLoad(IndicatorWidgetClass, [this, IndicatorPtr, IndicatorWidgetClass]()
		{
			if (UTPSIndicatorDescriptor* Indicator = IndicatorPtr.Get())
			{
				if (AllIndicators.Contains(IndicatorPtr) == false)
				{
					return;
				}

				if (UUserWidget* IndicatorWidget = IndicatorPool.GetOrCreateInstance(
					TSubclassOf<UUserWidget>(IndicatorWidgetClass.Get())))
				{
					if (IndicatorWidget->GetClass()->ImplementsInterface(UTPSIndicatorWidgetInterface::StaticClass()))
					{
						ITPSIndicatorWidgetInterface* InterfaceIndicator = Cast<ITPSIndicatorWidgetInterface>(
							IndicatorWidget);
						InterfaceIndicator->BindIndicator(Indicator);
					}
					Indicator->IndicatorWidget = IndicatorWidget;

					InactiveIndicators.Remove(Indicator);

					AddActorSlot(Indicator)
					[
						SAssignNew(Indicator->CanvasHost, SBox)
						[
							IndicatorWidget->TakeWidget()
						]
					];
				}
			}
		});
		StartAsyncLoading();
	}
}

void STPSActorCanvas::RemoveIndicatorForEntry(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	if (UUserWidget* IndicatorWidget = IndicatorDescriptor->IndicatorWidget.Get())
	{
		if (IndicatorWidget->GetClass()->ImplementsInterface(UTPSIndicatorWidgetInterface::StaticClass()))
		{
			ITPSIndicatorWidgetInterface* InterfaceIndicator = Cast<ITPSIndicatorWidgetInterface>(
				IndicatorWidget);
			InterfaceIndicator->BindIndicator(IndicatorDescriptor);
		}

		IndicatorDescriptor->IndicatorWidget = nullptr;
		IndicatorPool.Release(IndicatorWidget);
	}

	TSharedPtr<SWidget> CanvasHost = IndicatorDescriptor->CanvasHost.Pin();
	if (CanvasHost.IsValid() == true)
	{
		RemoveActorSlot(CanvasHost.ToSharedRef());
		IndicatorDescriptor->CanvasHost.Reset();
	}
}

STPSActorCanvas::FScopedWidgetSlotArguments STPSActorCanvas::AddActorSlot(UTPSIndicatorDescriptor* Indicator)
{
	TWeakPtr<STPSActorCanvas> WeakCanvas = SharedThis(this);
	return FScopedWidgetSlotArguments(MakeUnique<FSlot>(Indicator), this->CanvasChildren, INDEX_NONE,
	                                  [WeakCanvas](const FSlot*, int32)
	                                  {
		                                  if (TSharedPtr<STPSActorCanvas> Canvas = WeakCanvas.Pin())
		                                  {
			                                  Canvas->UpdateActiveTimer();
		                                  }
	                                  });
}

int32 STPSActorCanvas::RemoveActorSlot(const TSharedRef<SWidget>& SlotWidget)
{
	for (int32 SlotIdx = 0; SlotIdx < AllIndicators.Num(); ++SlotIdx)
	{
		if (SlotWidget == CanvasChildren[SlotIdx].GetWidget())
		{
			CanvasChildren.Remove(SlotWidget);

			UpdateActiveTimer();
			return SlotIdx;
		}
	}
	return -1;
}


void STPSActorCanvas::SetShowAnyIndicators(bool ShowAnyIndicators)
{
	if (bShowAnyIndicators != ShowAnyIndicators)
	{
		bShowAnyIndicators = ShowAnyIndicators;

		if (bShowAnyIndicators == false)
		{
			for (int32 ChildIndex = 0; ChildIndex < AllChildren.Num(); ++ChildIndex)
			{
				AllChildren.GetChildAt(ChildIndex)->SetVisibility(EVisibility::Collapsed);
			}
		}
	}
}

EActiveTimerReturnType STPSActorCanvas::UpdateCanvas(double InCurrentTime, float InDeltaTime)
{
	if (OptionalPaintGeometry.IsSet() == true)
	{
		return EActiveTimerReturnType::Continue;
	}

	ULocalPlayer* LocalPlayer = LocalPlayerContext.GetLocalPlayer();
	UTPSIndicatorManagerComponent* IC = IndicatorComp.Get();
	if (IndicatorComp == nullptr)
	{
		IndicatorComp = UTPSIndicatorManagerComponent::GetComponent(LocalPlayerContext.GetPlayerController());
		if (IndicatorComp != nullptr)
		{
			IndicatorPool.SetWorld(LocalPlayerContext.GetWorld());

			IndicatorComp = IC;
			IC->OnIndicatorAdded.AddSP(this, &STPSActorCanvas::OnIndicatorAdded);
			IC->OnIndicatorRemoved.AddSP(this, &STPSActorCanvas::OnIndicatorRemoved);
			for (UTPSIndicatorDescriptor* IndicatorDescriptor : IC->GetIndicators())
			{
				OnIndicatorAdded(IndicatorDescriptor);
			}
		}
		else
		{
			return EActiveTimerReturnType::Continue;
		}
	}

	if (LocalPlayer != nullptr)
	{
		const FGeometry PaintGeometry = OptionalPaintGeometry.GetValue();

		FSceneViewProjectionData ProjectionData;

		if (LocalPlayer->GetProjectionData(LocalPlayer->ViewportClient->Viewport, ProjectionData) == true)
		{
			SetShowAnyIndicators(true);

			bool bIndicatorsChanged = false;
			for (int32 ChildIndex = 0; ChildIndex < CanvasChildren.Num(); ++ChildIndex)
			{
				STPSActorCanvas::FSlot& CurChild = CanvasChildren[ChildIndex];
				UTPSIndicatorDescriptor* Indicator = CurChild.Indicator;

				if (Indicator->CanAutomaticallyRemove())
				{
					bIndicatorsChanged = true;
					RemoveIndicatorForEntry(Indicator);
					--ChildIndex;
					continue;
				}

				CurChild.SetIsIndicatorVisible(Indicator->GetIsVisible());

				if (CurChild.GetIsIndicatorVisible() == false)
				{
					bIndicatorsChanged |= CurChild.IsDirty();
					CurChild.ClearDirtyFlag();
					continue;
				}

				if (CurChild.IsIndicatorClampedStatusChanged() == true)
				{
					CurChild.ClearIndicatorClampedStatusChanged();
					bIndicatorsChanged = true;
				}

				FVector ScreenPositionWithDepth;
				FIndicatorProjection Projector;
				const bool bSuccess = Projector.Project(*Indicator, ProjectionData, PaintGeometry.Size,
				                                        OUT ScreenPositionWithDepth);
				if (bSuccess == false)
				{
					CurChild.SetHasValidScreenPosition(false);
					CurChild.SetInFrontOfCamera(false);

					bIndicatorsChanged |= CurChild.IsDirty();
					CurChild.ClearDirtyFlag();
					continue;
				}

				CurChild.SetInFrontOfCamera(bSuccess);
				CurChild.SetHasValidScreenPosition(CurChild.GetInFrontOfCamera() || Indicator->GetClampToScreen());

				if (CurChild.HasValidScreenPosition() == true)
				{
					CurChild.SetScreenPosition(FVector2D(ScreenPositionWithDepth.X, ScreenPositionWithDepth.Y));;
					CurChild.SetDepth(ScreenPositionWithDepth.X);
				}

				CurChild.SetPriority(Indicator->GetPriority());

				bIndicatorsChanged |= CurChild.IsDirty();
				CurChild.ClearDirtyFlag();
			}

			if (bIndicatorsChanged == true)
			{
				Invalidate(EInvalidateWidgetReason::Paint);
			}
		}
		else
		{
			SetShowAnyIndicators(false);
		}
	}
	else
	{
		SetShowAnyIndicators(false);
	}

	if (AllIndicators.Num() == 0)
	{
		TickHandle.Reset();
		return EActiveTimerReturnType::Stop;
	}
	else
	{
		return EActiveTimerReturnType::Continue;
	}
}

void STPSActorCanvas::GetOffsetAndSize(const UTPSIndicatorDescriptor* Indicator,
                                       FVector2D& OutSize,
                                       FVector2D& OutOffset,
                                       FVector2D& OutPaddingMin,
                                       FVector2D& OutPaddingMax) const
{
	FVector2D AllottedSize = FVector2D::ZeroVector;

	TSharedPtr<SWidget> CanvasHost = Indicator->CanvasHost.Pin();
	if (CanvasHost.IsValid() == true)
	{
		OutSize = CanvasHost->GetDesiredSize();
	}

	switch (Indicator->GetHorizontalAlign())
	{
	case HAlign_Left:
		{
			OutOffset.X = 0.0f;
			OutPaddingMin.X = OutSize.X;
			OutPaddingMax.X = 0.0f;
			break;
		}
	default:
	case HAlign_Center:
		{
			OutOffset.X = (AllottedSize.X - OutSize.X) * 0.5f;
			OutPaddingMin.X = OutSize.X * 0.5f;
			OutPaddingMax.X = OutPaddingMin.X;
			break;
		}
	case HAlign_Right:
		{
			OutOffset.X = AllottedSize.X - OutSize.X;
			OutPaddingMin.X = OutSize.X;
			OutPaddingMax.X = 0.0f;
			break;
		}
	}

	switch (Indicator->GetVerticalAlign())
	{
	case VAlign_Top:
		{
			OutOffset.Y = 0.0f;
			OutPaddingMin.Y = 0.0f;
			OutPaddingMax.Y = OutSize.Y;
			break;
		}
	default:
	case VAlign_Center:
		{
			OutOffset.Y = (AllottedSize.Y - OutSize.Y) * 0.5f;
			OutPaddingMin.Y = OutSize.Y * 0.5f;
			OutPaddingMax.Y = OutPaddingMin.Y;
			break;
		}
	case VAlign_Bottom:
		{
			OutOffset.Y = AllottedSize.Y - OutSize.Y;
			OutPaddingMin.Y = OutSize.Y;
			OutPaddingMax.Y = 0.0f;
			break;
		}
	}
}

void STPSActorCanvas::UpdateActiveTimer()
{
	const bool NeedsTicks = AllIndicators.Num() > 0 || IndicatorComp.IsValid() == false;;
	if (NeedsTicks == true && TickHandle.IsValid() == true)
	{
		TickHandle = RegisterActiveTimer(0, FWidgetActiveTimerDelegate::CreateSP(this, &STPSActorCanvas::UpdateCanvas));
	}
}
