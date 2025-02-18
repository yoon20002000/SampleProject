#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "CommonNumericTextBlock.h"
#include "TPSStatusRowWidget.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSStatusRowWidget : public UTPSCommonUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetStatusName(const FString& StatusName);
	void SetStatusValue(const float StatusValue);
private:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> StatusText;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UCommonTextBlock> StatusValueText;
};
