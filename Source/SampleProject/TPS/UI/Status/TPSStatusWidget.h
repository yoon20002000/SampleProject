#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSStatusWidget.generated.h"

class UTPSStatusRowWidget;
class UTPSInventoryComponent;
class UTPSHealthComponent;
class UScrollBox;
class UImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSStatusWidget : public UTPSCommonUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void Init(ATPSPlayer* PlayerActor);
private:
	void SetPlayerName(const FString& PlayerName);
	// ScrollBox에 추가 될 Row 추가용
	void AddChild(const FString& StatusName, const FString& StatusValue);
	void RemoveChild(const int32 RowIndex);
	UFUNCTION()
	void UpdateHealthUI(UTPSHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* Instigator);
	// HP 관련 Status
	void InitHPStatus(UTPSHealthComponent* HealthComp);
	void UnInitHPStatus(UTPSHealthComponent* HealthComp);
	// Inventory 관련 Status
	void InitInventoryStatus(const UTPSInventoryComponent* InventoryComp);
	void UnInitInventoryStatus(const UTPSInventoryComponent* InventoryComp);
	void UpdateUI();
private:
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> PlayerNameText;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> RTPlayerImage;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollBox> StatusScrollBox;

	// Status에 추가 될 때 사용 할 Row WidgetClass
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSoftClassPtr<UTPSStatusRowWidget> StatusRowClass;

	UPROPERTY()
	TArray<TObjectPtr<UTPSStatusRowWidget>> StatusRows;
	
	TWeakObjectPtr<ATPSPlayer> TargetPlayer;
};
	
