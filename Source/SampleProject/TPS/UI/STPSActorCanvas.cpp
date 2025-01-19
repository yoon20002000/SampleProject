// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STPSActorCanvas.h"
#include "TPSIndicatorDescriptor.h"
#include "TPSIndicatorWidgetInterface.h"
#include "Components/TPSIndicatorManagerComponent.h"

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
				const bool bSuccess = Projector.Project(*Indicator, ProjectionData, PaintGeometry.Size, OUT ScreenPositionWithDepth);
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

void STPSActorCanvas::UpdateActiveTimer()
{
	const bool NeedsTicks = AllIndicators.Num() > 0 || IndicatorComp.IsValid() == false;;
	if (NeedsTicks == true && TickHandle.IsValid() == true)
	{
		TickHandle = RegisterActiveTimer(0, FWidgetActiveTimerDelegate::CreateSP(this, &STPSActorCanvas::UpdateCanvas));
	}
}
