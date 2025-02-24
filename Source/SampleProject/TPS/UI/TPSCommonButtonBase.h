#pragma once

#include "CommonButtonBase.h"
#include "TPSCommonButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UTPSCommonButtonBase();
	void SetButtonText(const FText& NewText);
private:
	UPROPERTY(meta=(AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UCommonTextBlock> ButtonTextBlock;
};
