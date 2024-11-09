

#include "TPSSystemManager.h"

UTPSSystemManager* UTPSSystemManager::Instance = nullptr;

UTPSSystemManager::UTPSSystemManager(const FObjectInitializer& objectInitializer)
	:Super(objectInitializer)
{
	Instance = this;
}

UTPSSystemManager* UTPSSystemManager::CreateInstance()
{
	if(!Instance)
	{
		Instance = NewObject<UTPSSystemManager>();
	}
	return Instance;
}

void UTPSSystemManager::DestoryInstance()
{	
	Instance = nullptr;
}

UTPSSystemManager* UTPSSystemManager::Get()
{
	return Instance;
}

void UTPSSystemManager::Tick(float DeltaTime)
{
}

void UTPSSystemManager::PostTick(float DeltaTime)
{
}
