#pragma once

#include "TPSIndicatorWidgetInterface.h"
#include "UI/TPSCommonUserWidget.h"
#include "TPSInteractionIndicatorWidget.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSInteractionIndicatorWidget : public UTPSCommonUserWidget, public ITPSIndicatorWidgetInterface
{
	GENERATED_BODY()
public:
	void SetInteractionText(const FText& InteractionText);

	// ITPSIndicatorWidgetInterface
	virtual void BindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor) override;
	virtual void UnbindIndicator(UTPSIndicatorDescriptor* IndicatorDescriptor) override;
private:
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess=true,BindWidget))
	TObjectPtr<UCommonTextBlock> InteractionDescriptionText;
};
