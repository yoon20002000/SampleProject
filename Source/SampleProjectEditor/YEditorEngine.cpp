#include "YEditorEngine.h"
#include "YSystemManager.h"


void UYEditorEngine::Init(class IEngineLoop* InEngineLoop)
{

	SystemManager = UYSystemManager::CreateInstance();

	Super::Init(InEngineLoop);

}

void UYEditorEngine::PreExit()
{
	Super::PreExit();
	
	UYSystemManager::DestoryInstance();
	SystemManager = nullptr;
}

void UYEditorEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	if (UYSystemManager::Get())
	{
		UYSystemManager::Get()->Tick(DeltaSeconds);
	}

	Super::Tick(DeltaSeconds, bIdleMode);

	if (UYSystemManager::Get())
	{
		UYSystemManager::Get()->PostTick(DeltaSeconds);
	}

}

