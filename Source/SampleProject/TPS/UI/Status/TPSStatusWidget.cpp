#include "UI/Status/TPSStatusWidget.h"
#include "CommonTextBlock.h"
#include "TPSStatusRowWidget.h"
#include "Character/TPSCharacter.h"
#include "Character/TPSPlayer.h"
#include "Components/ScrollBox.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSInventoryComponent.h"
#include "GameFramework/Character.h"

void UTPSStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTPSStatusWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTPSStatusWidget::Init(const ATPSPlayer& PlayerActor)
{
	SetPlayerName(PlayerActor.GetName());
	SetStatus(PlayerActor);
}

void UTPSStatusWidget::SetPlayerName(const FString& PlayerName)
{
	PlayerNameText->SetText(FText::FromString(PlayerName));
}

void UTPSStatusWidget::SetStatus(const ATPSPlayer& Player)
{
	SetHPStatus(Player.GetHealthComponentOrNullptr());
	SetInventoryStatus(Player.GetInventoryComponent());
}

void UTPSStatusWidget::AddChild(const FString& StatusName, const FString& StatusValue)
{
	UTPSStatusRowWidget* Row = CreateWidget<UTPSStatusRowWidget>(StatusScrollBox,StatusRowClass.LoadSynchronous());
	Row->SetStatusName(StatusName);
	Row->SetStatusValue(StatusValue);
	StatusScrollBox->AddChild(Row);
}

void UTPSStatusWidget::SetHPStatus(const UTPSHealthComponent* HealthComp)
{
	if (HealthComp == nullptr)
	{
		return;
	}
	
	AddChild(TEXT("Health : "), FString::SanitizeFloat(HealthComp->GetHealth()));
	AddChild(TEXT("Max Health : "), FString::SanitizeFloat(HealthComp->GetMaxHealth()));
}

void UTPSStatusWidget::SetInventoryStatus(const UTPSInventoryComponent* InventoryComp)
{
	if (InventoryComp == nullptr)
	{
		return;
	}

	int32 SlotSize = InventoryComp->GetInventorySlotSize();
	AddChild(TEXT("Inventory Size : "), FString::FromInt(SlotSize));
}
