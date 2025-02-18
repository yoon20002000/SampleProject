#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSStatusWidget.generated.h"

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

	void Init(const AActor& PlayerActor);
private:
	void SetPlayerName(const FString& PlayerName);
private:
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> PlayerNameText;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> RTPlayerImage;
	UPROPERTY(EditAnywhere,meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UScrollBox> StatusScrollBox;

	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess))
	TSoftClassPtr<UTPSCommonUserWidget> StatusRowClass;
};
	
