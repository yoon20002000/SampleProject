#include "TPSEditorEngine.h"
#include "TPSSystemManager.h"


void UTPSEditorEngine::Init(class IEngineLoop* InEngineLoop)
{

	SystemManager = UTPSSystemManager::CreateInstance();
	
	Super::Init(InEngineLoop);

	SystemManager->Initialize();
}

void UTPSEditorEngine::PreExit()
{
	Super::PreExit();
	
	UTPSSystemManager::Get()->Deinitialize();
	UTPSSystemManager::DestoryInstance();
	SystemManager = nullptr;
}

void UTPSEditorEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	if (UTPSSystemManager::Get())
	{
		UTPSSystemManager::Get()->Tick(DeltaSeconds);
	}

	Super::Tick(DeltaSeconds, bIdleMode);

	if (UTPSSystemManager::Get())
	{
		UTPSSystemManager::Get()->PostTick(DeltaSeconds);
	}

}

