#include "UI/TPSRespawnWidget.h"

#include "CommonNumericTextBlock.h"

UTPSRespawnWidget::UTPSRespawnWidget() : CountdownPeriod(.1f), CountdownTime(5)
{
}

void UTPSRespawnWidget::StartCountDown(float InCountdownTime)
{
	if (InCountdownTime > 0)
	{
		CountdownTime = InCountdownTime;
	}
	UpdateCountdownText(CountdownTime);
	SetVisibility(ESlateVisibility::Visible);
	FTimerDelegate CountDownDelegate;
	CountDownDelegate.BindLambda(
		[this]( )
		{
			float ElapsedTime =  GetWorld()->GetTimerManager().GetTimerElapsed(CountDownTimerHandle);
			if (ElapsedTime > CountdownTime)
			{
				StopCountDown();
			}
			UpdateCountdownText(CountdownTime - ElapsedTime);
		});
	GetWorld()->GetTimerManager().SetTimer(CountDownTimerHandle, CountDownDelegate, CountdownPeriod, true);
}

void UTPSRespawnWidget::StopCountDown()
{
	SetVisibility(ESlateVisibility::Collapsed);
	GetWorld()->GetTimerManager().ClearTimer(CountDownTimerHandle);
}

void UTPSRespawnWidget::UpdateCountdownText(float Time)
{
	CountdownText->SetText(FText::AsNumber(Time));
}
