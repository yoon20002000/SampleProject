#include "UI/Status/TPSStatusWidget.h"
#include "CommonTextBlock.h"

void UTPSStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTPSStatusWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTPSStatusWidget::Init(const AActor& PlayerActor)
{
	SetPlayerName(PlayerActor.GetName());
}

void UTPSStatusWidget::SetPlayerName(const FString& PlayerName)
{
	PlayerNameText->SetText(FText::FromString(PlayerName));
}
