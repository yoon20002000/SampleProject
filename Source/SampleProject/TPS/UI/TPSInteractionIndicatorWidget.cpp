#include "UI/TPSInteractionIndicatorWidget.h"

#include "CommonTextBlock.h"

void UTPSInteractionIndicatorWidget::SetInteractionText(const FText& InteractionText)
{
	InteractionDescriptionText->SetText(InteractionText);
}
