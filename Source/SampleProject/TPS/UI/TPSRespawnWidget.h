#pragma once

#include "CoreMinimal.h"
#include "UI/TPSCommonActivatableWidget.h"
#include "TPSRespawnWidget.generated.h"

class UCommonNumericTextBlock;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSRespawnWidget : public UTPSCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UTPSRespawnWidget();
	void StartCountDown(float InCountdownTime = -1.f);
	void StopCountDown();
private:
	void UpdateCountdownText(float Time);
private:
	UPROPERTY(EditAnywhere, meta=(allowPrivateAccess=true, BindWidget))
	TObjectPtr<UCommonNumericTextBlock> CountdownText;
	UPROPERTY(EditAnywhere, Transient)
	float CountdownPeriod;
	UPROPERTY(EditAnywhere, Transient)
	float CountdownTime;
	FTimerHandle CountDownTimerHandle;
};
