#pragma once

#include "CoreMinimal.h"
#include "Editor/UnrealEdEngine.h"
#include "TPSEditorEngine.generated.h"

class UTPSSystemManager;

UCLASS()
class UTPSEditorEngine : public UUnrealEdEngine
{
	GENERATED_BODY()

public:
	virtual void Init(class IEngineLoop* InEngineLoop) override;
	virtual void PreExit() override;
	virtual void Tick(float DeltaSeconds, bool bIdleMode) override;

	
private:

	UPROPERTY()
	UTPSSystemManager* SystemManager = nullptr;
};
