#pragma once

#include "UI/TPSCommonUserWidget.h"
#include "TPSInteractionIndicatorWidget.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSInteractionIndicatorWidget : public UTPSCommonUserWidget
{
	GENERATED_BODY()
public:
	void SetInteractionText(const FText& InteractionText);
private:
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true,BindWidget))
	TObjectPtr<UCommonTextBlock> InteractionDescriptionText;
};
