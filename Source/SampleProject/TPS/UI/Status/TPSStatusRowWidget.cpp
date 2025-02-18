#include "UI/Status/TPSStatusRowWidget.h"

void UTPSStatusRowWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTPSStatusRowWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTPSStatusRowWidget::SetStatusName(const FString& StatusName)
{
	StatusText->SetText(FText::FromString(StatusName));
}

void UTPSStatusRowWidget::SetStatusValue(const float StatusValue)
{
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.SetMinimumFractionalDigits(2);
	FormattingOptions.SetMaximumFractionalDigits(2);

	StatusValueText->SetText(FText::AsNumber(StatusValue, &FormattingOptions));
}
