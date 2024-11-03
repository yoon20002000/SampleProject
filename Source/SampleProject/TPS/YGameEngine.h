
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "YGameEngine.generated.h"

class UYSystemManager;

UCLASS()
class UYGameEngine : public UGameEngine
{
	GENERATED_BODY()

public:

	void InitializeClient();
	void ShutDownClient();

	virtual void Init(class IEngineLoop* InEngineLoop) override;
	virtual void Start() override;
	virtual void PreExit() override;
	virtual void Tick(float DeltaSeconds, bool bIdleMode) override;

	virtual bool LoadMap(FWorldContext& WorldContext, FURL URL, class UPendingNetGame* Pending, FString& Error);

	// Begin FExec Interface
	virtual bool		Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar = *GLog) override;
	// End FExec Interface

private:
	static void OnPreWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues IVS);
	static void OnPostWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues IVS);
	static void OnWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResources);

protected:

	bool bClientInitialized;

	UPROPERTY()
	UYSystemManager* SystemManager;

};
