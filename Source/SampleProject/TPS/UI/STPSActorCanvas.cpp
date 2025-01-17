// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STPSActorCanvas.h"

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
bool STPSActorCanvas::FSlot::GetIsDirty() const
{
	return bDirty;
}
void STPSActorCanvas::FSlot::ClearDirtyFlag()
{
	bDirty = false;
}
