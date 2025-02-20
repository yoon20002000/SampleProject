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
	StatusRows.Reserve(4);
}

void UTPSStatusWidget::NativeDestruct()
{
	UnInitHPStatus(TargetPlayer->GetHealthComponentOrNullptr());
	UnInitInventoryStatus(TargetPlayer->GetInventoryComp());
	Super::NativeDestruct();
}

void UTPSStatusWidget::Init(ATPSPlayer* PlayerActor)
{
	TargetPlayer = PlayerActor;
	SetPlayerName(PlayerActor->GetName());
	UpdateUI();
}

void UTPSStatusWidget::SetPlayerName(const FString& PlayerName)
{
	PlayerNameText->SetText(FText::FromString(PlayerName));
}

void UTPSStatusWidget::AddChild(const FString& StatusName, const FString& StatusValue)
{
	UTPSStatusRowWidget* Row = CreateWidget<UTPSStatusRowWidget>(StatusScrollBox,StatusRowClass.LoadSynchronous());
	Row->SetStatusName(StatusName);
	Row->SetStatusValue(StatusValue);
	StatusScrollBox->AddChild(Row);
	StatusRows.Add(Row);
}

void UTPSStatusWidget::RemoveChild(const int32 RowIndex)
{
	StatusRows.RemoveAt(RowIndex);
}

void UTPSStatusWidget::UpdateHealthUI(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue,
                                      AActor* Instigator)
{
	UpdateUI();
}

void UTPSStatusWidget::InitHPStatus(UTPSHealthComponent* HealthComp)
{
	if (HealthComp == nullptr)
	{
		return;
	}
	HealthComp->OnHealthChanged.AddDynamic(this, &ThisClass::UpdateHealthUI);
	HealthComp->OnMaxHealthChanged.AddDynamic(this, &ThisClass::UpdateHealthUI);
	
	AddChild(TEXT("Health : "), FString::SanitizeFloat(HealthComp->GetHealth()));
	AddChild(TEXT("Max Health : "), FString::SanitizeFloat(HealthComp->GetMaxHealth()));
}

void UTPSStatusWidget::UnInitHPStatus(UTPSHealthComponent* HealthComp)
{
	if (HealthComp == nullptr)
	{
		return;
	}
	
	HealthComp->OnHealthChanged.RemoveAll(this);
	HealthComp->OnMaxHealthChanged.RemoveAll(this);
}

void UTPSStatusWidget::InitInventoryStatus(const UTPSInventoryComponent* InventoryComp)
{
	if (InventoryComp == nullptr)
	{
		return;
	}

	int32 SlotSize = InventoryComp->GetInventorySlotSize();
	AddChild(TEXT("Inventory Size : "), FString::FromInt(SlotSize));
}

void UTPSStatusWidget::UnInitInventoryStatus(const UTPSInventoryComponent* InventoryComp)
{
	// 이후 필요시 작성
}

void UTPSStatusWidget::UpdateUI()
{
	StatusScrollBox->ClearChildren();
	if (UTPSHealthComponent* HealthComp = TargetPlayer->GetHealthComponentOrNullptr())
	{
		UnInitHPStatus(HealthComp);
		InitHPStatus(HealthComp);
	}
	if (UTPSInventoryComponent* InvenComp = TargetPlayer->GetInventoryComp())
	{
		UnInitInventoryStatus(InvenComp);
		InitInventoryStatus(InvenComp);
	}
}
