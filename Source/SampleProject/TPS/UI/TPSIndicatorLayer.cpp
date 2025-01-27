// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TPSIndicatorLayer.h"

#include "STPSActorCanvas.h"

UTPSIndicatorLayer::UTPSIndicatorLayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsVariable = true;
	UWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTPSIndicatorLayer::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	ActorCanvas.Reset();
}

TSharedRef<SWidget> UTPSIndicatorLayer::RebuildWidget()
{
	if (IsDesignTime() == false)
	{
		ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
		if (ensureMsgf(LocalPlayer, TEXT("Attempt to rebuild a UActorCanvas without a valid LocalPlayer")))
		{
			ActorCanvas = SNew(STPSActorCanvas, FLocalPlayerContext(LocalPlayer), &ArrowBrush);
			return ActorCanvas.ToSharedRef();
		}
	}
	return SNew(SBox);
}
