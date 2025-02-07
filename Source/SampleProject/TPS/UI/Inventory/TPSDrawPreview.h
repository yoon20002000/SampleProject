#pragma once

#include "Blueprint/UserWidget.h"
#include "TPSDrawPreview.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSDrawPreview : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPreview(const FName& InItemName);
	
private:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> PreviewItemIconImage;

	FName ItemName;
};
