

#include "YSystemManager.h"

UYSystemManager* UYSystemManager::Instance = nullptr;

UYSystemManager::UYSystemManager(const FObjectInitializer& objectInitializer)
	:Super(objectInitializer)
{
	Instance = this;
}

UYSystemManager* UYSystemManager::CreateInstance()
{
	if(!Instance)
	{
		Instance = NewObject<UYSystemManager>();
	}
	return Instance;
}

void UYSystemManager::DestoryInstance()
{	
	Instance = nullptr;
}

UYSystemManager* UYSystemManager::Get()
{
	return Instance;
}

void UYSystemManager::Tick(float DeltaTime)
{
}

void UYSystemManager::PostTick(float DeltaTime)
{
}
