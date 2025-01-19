// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSIndicatorDescriptor.h"

#include "Components/TPSIndicatorManagerComponent.h"


bool FIndicatorProjection::Project(const UTPSIndicatorDescriptor& IndicatorDescriptor,
                                   const FSceneViewProjectionData& InProjectionData, const FVector2f& ScreenSize,
                                   FVector& OutScreenPositionWithDepth)
{
	if (USceneComponent* TargetComponent = IndicatorDescriptor.GetTargetSceneComponent())
	{
		TOptional<FVector> WorldLocation;
		if (IndicatorDescriptor.GetComponentSocketName() != NAME_None)
		{
			WorldLocation = TargetComponent->GetSocketTransform(IndicatorDescriptor.GetComponentSocketName()).
			                                 GetLocation();
		}
		else
		{
			WorldLocation = TargetComponent->GetComponentLocation();
		}

		const FVector ProjectWorldLocation = WorldLocation.GetValue() + IndicatorDescriptor.GetWorldPositionOffset();

		switch (const EActorCanvasProjectionMode ProjectionMode = IndicatorDescriptor.GetProjectionMode())
		{
		case EActorCanvasProjectionMode::ComponentPoint:
			{
				if (WorldLocation.IsSet() == true)
				{
					FVector2D OutScreenSpacePosition;
					const bool bInFrontOfCamera = ULocalPlayer::GetPixelPoint(
						InProjectionData, ProjectWorldLocation, OutScreenSpacePosition, &ScreenSize);

					OutScreenSpacePosition.X += IndicatorDescriptor.GetScreenSpaceOffset().X * (
						bInFrontOfCamera == true ? 1 : -1);
					OutScreenSpacePosition.Y += IndicatorDescriptor.GetScreenSpaceOffset().Y;

					if (bInFrontOfCamera == false && FBox2f(FVector2f::Zero(), ScreenSize).IsInside(
						static_cast<FVector2f>(OutScreenSpacePosition)) == true)
					{
						const FVector2f CenterToPosition = (FVector2f(OutScreenSpacePosition) - (ScreenSize / 2)).
							GetSafeNormal();
						OutScreenSpacePosition = FVector2D((ScreenSize / 2) + CenterToPosition * ScreenSize);
					}
					OutScreenPositionWithDepth = FVector(OutScreenSpacePosition.X, OutScreenSpacePosition.Y,
					                                     FVector::Dist(InProjectionData.ViewOrigin,
					                                                   ProjectWorldLocation));
					return true;
				}
				else
				{
					return false;
				}
			}
		case EActorCanvasProjectionMode::ComponentScreenBoundingBox:
		case EActorCanvasProjectionMode::ActorScreenBoundingBox:
			{
				FBox IndicatorBox;
				if (ProjectionMode == EActorCanvasProjectionMode::ActorScreenBoundingBox)
				{
					IndicatorBox = TargetComponent->GetOwner()->GetComponentsBoundingBox();
				}
				else
				{
					IndicatorBox = TargetComponent->Bounds.GetBox();
				}

				FVector2D LL, UR;
				const bool bInFrontOfCamera = ULocalPlayer::GetPixelBoundingBox(
					InProjectionData, IndicatorBox, LL, UR, &ScreenSize);

				const FVector& BoundingBoxAnchor = IndicatorDescriptor.GetBoundingBoxAnchor();
				const FVector2D ScreenSpaceOffset = IndicatorDescriptor.GetScreenSpaceOffset();

				FVector ScreenPositionWithDepth;
				ScreenPositionWithDepth.X = FMath::Lerp(LL.X, UR.X, BoundingBoxAnchor.X) + ScreenSpaceOffset.X * (
					bInFrontOfCamera == true ? 1 : -1);
				ScreenPositionWithDepth.Y = FMath::Lerp(LL.Y, UR.Y, BoundingBoxAnchor.Y) + ScreenSpaceOffset.Y;
				ScreenPositionWithDepth.Z = FVector::Dist(InProjectionData.ViewOrigin, ProjectWorldLocation);

				const FVector2f ScreenSpacePosition = FVector2f(FVector2D(ScreenPositionWithDepth));
				if (bInFrontOfCamera == false && FBox2f(FVector2f::Zero(), ScreenSize).IsInside(ScreenSpacePosition) ==
					true)
				{
					const FVector2f CenterToPosition = (ScreenSpacePosition - (ScreenSize / 2)).GetSafeNormal();
					const FVector2f ScreenPositionFromBehind = (ScreenSize / 2) + CenterToPosition * ScreenSize;
					ScreenPositionWithDepth.X = ScreenPositionFromBehind.X;
					ScreenPositionWithDepth.Y = ScreenPositionFromBehind.Y;
				}
				OutScreenPositionWithDepth = ScreenPositionWithDepth;
				return true;
			}
		case EActorCanvasProjectionMode::ActorBoundingBox:
		case EActorCanvasProjectionMode::ComponentBoundingBox:
			{
				FBox IndicatorBox;
				if (ProjectionMode == EActorCanvasProjectionMode::ActorBoundingBox)
				{
					IndicatorBox = TargetComponent->GetOwner()->GetComponentsBoundingBox();
				}
				else
				{
					IndicatorBox = TargetComponent->Bounds.GetBox();
				}

				const FVector ProjectBoxPoint = IndicatorBox.GetCenter() + (IndicatorBox.GetSize() * (
					IndicatorDescriptor.GetBoundingBoxAnchor() - FVector(0.5f)));

				FVector2D OutScreenSpacePosition;
				const bool bInFrontOfCamera = ULocalPlayer::GetPixelPoint(InProjectionData, ProjectBoxPoint, OutScreenSpacePosition, &ScreenSize);
				OutScreenSpacePosition.X += IndicatorDescriptor.GetScreenSpaceOffset().X * (bInFrontOfCamera == true ? 1 : -1);
				OutScreenSpacePosition.Y += IndicatorDescriptor.GetScreenSpaceOffset().Y;

				if (bInFrontOfCamera == false && FBox2f(FVector2f::Zero(), ScreenSize).IsInside(
					static_cast<FVector2f>(OutScreenSpacePosition)) == true)
				{
					const FVector2f CenterToPosition = (FVector2f(OutScreenSpacePosition) - (ScreenSize / 2)).
					                                   GetSafeNormal().GetSafeNormal();
					OutScreenSpacePosition = FVector2D((ScreenSize / 2) + CenterToPosition * ScreenSize);
				}
				OutScreenPositionWithDepth = FVector(OutScreenSpacePosition.X, OutScreenSpacePosition.Y,
				                                     FVector::Dist(InProjectionData.ViewOrigin, ProjectBoxPoint));
				return true;
			}
		}
	}
	return false;
}

UTPSIndicatorDescriptor::UTPSIndicatorDescriptor()
{
}

UObject* UTPSIndicatorDescriptor::GetDataObject() const
{
	return DataObject;
}

void UTPSIndicatorDescriptor::SetDataObject(UObject* InDataObject)
{
	DataObject = InDataObject;
}

USceneComponent* UTPSIndicatorDescriptor::GetTargetSceneComponent() const
{
	return TargetSceneComponent;
}

void UTPSIndicatorDescriptor::SetTargetSceneComponent(USceneComponent* InSceneComponent)
{
	TargetSceneComponent = InSceneComponent;
}

FName UTPSIndicatorDescriptor::GetComponentSocketName() const
{
	return ComponentSocketName;
}

void UTPSIndicatorDescriptor::SetComponentSocketName(FName InSocketName)
{
	ComponentSocketName = InSocketName;
}

TSoftClassPtr<UUserWidget> UTPSIndicatorDescriptor::GetIndicatorWidgetClass() const
{
	return IndicatorWidgetClass;
}

void UTPSIndicatorDescriptor::SetIndicatorClass(TSoftClassPtr<UUserWidget> InIndicatorClass)
{
	IndicatorWidgetClass = InIndicatorClass;
}

UTPSIndicatorManagerComponent* UTPSIndicatorDescriptor::GetIndicatorManagerComponent() const
{
	return IndicatorManagerComp.Get();
}

void UTPSIndicatorDescriptor::SetIndicatorManagerComponent(UTPSIndicatorManagerComponent* InManager)
{
	if (ensure(IndicatorManagerComp.IsExplicitlyNull()))
	{
		IndicatorManagerComp = InManager;
	}
}

void UTPSIndicatorDescriptor::UnregisterIndicator()
{
	if (UTPSIndicatorManagerComponent* Manager = IndicatorManagerComp.Get())
	{
		Manager->RemoveIndicator(this);
	}
}

bool UTPSIndicatorDescriptor::GetAutoRemoveWhenIndicatorComponentIsNull() const
{
	return bAutoRemoveWhenIndicatorComponentIsNull;
}

void UTPSIndicatorDescriptor::SetAutoRemoveWhenIndicatorComponentIsNull(bool CanAutomaticallyRemove)
{
	bAutoRemoveWhenIndicatorComponentIsNull = CanAutomaticallyRemove;
}

bool UTPSIndicatorDescriptor::CanAutomaticallyRemove() const
{
	return IsValid(GetTargetSceneComponent()) == true && bAutoRemoveWhenIndicatorComponentIsNull == true;
}

bool UTPSIndicatorDescriptor::GetIsVisible() const
{
	return IsValid(GetTargetSceneComponent()) == true && bVisible;
}

void UTPSIndicatorDescriptor::SetDesiredVisibility(bool InVisible)
{
	bVisible = InVisible;
}

EActorCanvasProjectionMode UTPSIndicatorDescriptor::GetProjectionMode() const
{
	return ProjectionMode;
}

void UTPSIndicatorDescriptor::SetProjectionMode(EActorCanvasProjectionMode InProjectionMode)
{
	ProjectionMode = InProjectionMode;
}

EHorizontalAlignment UTPSIndicatorDescriptor::GetHorizontalAlign() const
{
	return HAlignment;
}

void UTPSIndicatorDescriptor::SetHorizontalAlign(EHorizontalAlignment InHorizontalAlignment)
{
	HAlignment = InHorizontalAlignment;
}

EVerticalAlignment UTPSIndicatorDescriptor::GetVerticalAlign() const
{
	return VAlignment;
}

void UTPSIndicatorDescriptor::SetVerticalAlign(EVerticalAlignment InVerticalAlignment)
{
	VAlignment = InVerticalAlignment;
}

bool UTPSIndicatorDescriptor::GetClampToScreen() const
{
	return bClampToScreen;
}

void UTPSIndicatorDescriptor::SetClampToScreen(bool InClampToScreen)
{
	bShowClampToScreenArrow = InClampToScreen;
}

bool UTPSIndicatorDescriptor::GetShowClampToScreenArrow() const
{
	return bShowClampToScreenArrow;
}

void UTPSIndicatorDescriptor::SetShowClampToScreenArrow(bool InShowClampToScreenArrow)
{
	bShowClampToScreenArrow = InShowClampToScreenArrow;
}

FVector UTPSIndicatorDescriptor::GetWorldPositionOffset() const
{
	return WorldPositionOffset;
}

void UTPSIndicatorDescriptor::SetWorldPositionOffset(FVector WorldPosOffset)
{
	WorldPositionOffset = WorldPosOffset;
}

FVector2D UTPSIndicatorDescriptor::GetScreenSpaceOffset() const
{
	return ScreenSpaceOffset;
}

void UTPSIndicatorDescriptor::SetScreenSpaceOffset(FVector2D InScreenSpaceOffset)
{
	ScreenSpaceOffset = InScreenSpaceOffset;
}

FVector UTPSIndicatorDescriptor::GetBoundingBoxAnchor() const
{
	return BoundingBoxAnchor;
}

void UTPSIndicatorDescriptor::SetBoundingBoxAnchor(FVector InBoundingBoxAnchor)
{
	BoundingBoxAnchor = InBoundingBoxAnchor;
}

int32 UTPSIndicatorDescriptor::GetPriority()
{
	return Priority;
}

void UTPSIndicatorDescriptor::SetPriority(int32 InPriority)
{
	Priority = InPriority;
}
