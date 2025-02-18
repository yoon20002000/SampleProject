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

void UTPSStatusWidget::SetHPStatus(const UTPSHealthComponent* HealthComp)
{
	if (HealthComp == nullptr)
	{
		return;
	}
	
	float HP = HealthComp->GetHealth();
	float MaxHP = HealthComp->GetMaxHealth();

	UTPSStatusRowWidget* HealthRow = CreateWidget<UTPSStatusRowWidget>(StatusScrollBox,StatusRowClass.LoadSynchronous());
	HealthRow->SetStatusName(TEXT("Health : "));
	HealthRow->SetStatusValue(FString::SanitizeFloat(HP));
	StatusScrollBox->AddChild(HealthRow);
	
	UTPSStatusRowWidget* MaxHealthRow = CreateWidget<UTPSStatusRowWidget>(StatusScrollBox,StatusRowClass.LoadSynchronous());
	MaxHealthRow->SetStatusName(TEXT("Max Health: "));
	MaxHealthRow->SetStatusValue(FString::SanitizeFloat(MaxHP));
	StatusScrollBox->AddChild(MaxHealthRow);
}

void UTPSStatusWidget::SetInventoryStatus(const UTPSInventoryComponent* InventoryComp)
{
	if (InventoryComp == nullptr)
	{
		return;
	}

	UTPSStatusRowWidget* InventorySizeRow = CreateWidget<UTPSStatusRowWidget>(StatusScrollBox,StatusRowClass.LoadSynchronous());

	int32 SlotSize = InventoryComp->GetInventorySlotSize();
	InventorySizeRow->SetStatusName(TEXT("Inventory Size: "));
	InventorySizeRow->SetStatusValue(FString::FromInt(SlotSize));

	StatusScrollBox->AddChild(InventorySizeRow);
}
