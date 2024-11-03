#pragma once

#include "CoreMinimal.h"
#include "Editor/UnrealEdEngine.h"
#include "YEditorEngine.generated.h"

class UYSystemManager;

UCLASS()
class UYEditorEngine : public UUnrealEdEngine
{
	GENERATED_BODY()

public:
	virtual void Init(class IEngineLoop* InEngineLoop) override;
	virtual void PreExit() override;
	virtual void Tick(float DeltaSeconds, bool bIdleMode) override;

	
private:

	UPROPERTY()
	UYSystemManager* SystemManager = nullptr;
};
