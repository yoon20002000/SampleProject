#include "UI/TPSInteractionIndicatorWidget.h"

#include "CommonTextBlock.h"
#include "TPSIndicatorDescriptor.h"
#include "Actor/TPSInteractionActor.h"

void UTPSInteractionIndicatorWidget::SetInteractionText(const FText& InteractionText)
{
	InteractionDescriptionText->SetText(InteractionText);
}

void UTPSInteractionIndicatorWidget::BindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Indicator Descriptor is nullptr!!"));
		return;
	}
	
	ATPSInteractionActor* InteractionActor = Cast<ATPSInteractionActor>(IndicatorDescriptor->GetDataObject());
	if (InteractionActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Indicator Descriptor Data is nullptr!!"));
		return;
	}

	SetInteractionText(FText::FromString(InteractionActor->GetInteractionDescription()));
}

void UTPSInteractionIndicatorWidget::UnbindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor)
{
		
}
