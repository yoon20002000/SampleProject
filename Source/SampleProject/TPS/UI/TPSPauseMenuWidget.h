#pragma once

#include "UI/TPSCommonActivatableWidget.h"
#include "TPSPauseMenuWidget.generated.h"

class UTPSCommonButtonBase;
class UCommonButtonBase;
class UButton;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSPauseMenuWidget : public UTPSCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool Initialize() override;
	virtual void BeginDestroy() override;
	
	UFUNCTION()
	void OnClickedOption();
	UFUNCTION()
	void OnClickedQuit();
	UFUNCTION()
	void OnClickedContinue();
	void SetGamePause(const bool bPauseGame);
private:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTPSCommonButtonBase> OptionButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTPSCommonButtonBase> QuitButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTPSCommonButtonBase> ContinueButton;
	
};
