#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSStatusWidget.generated.h"

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

	void Init(const ATPSPlayer& PlayerActor);
private:
	void SetPlayerName(const FString& PlayerName);
	void SetStatus(const ATPSPlayer& Player);
	// ScrollBox에 추가 될 Row 추가용
	void AddChild(const FString& StatusName, const FString& StatusValue);
	// HP 관련 Status
	void SetHPStatus(const UTPSHealthComponent* HealthComp);
	// Inventory 관련 Status
	void SetInventoryStatus(const UTPSInventoryComponent* InventoryComp);
private:
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> PlayerNameText;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> RTPlayerImage;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollBox> StatusScrollBox;

	// Status에 추가 될 때 사용 할 Row WidgetClass
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSoftClassPtr<UTPSCommonUserWidget> StatusRowClass;
};
	
