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

void UTPSStatusRowWidget::SetStatusValue(const FString& FormatString)
{
	StatusValueText->SetText(FText::FromString(FormatString));
}
