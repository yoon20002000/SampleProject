#include "TPSSystemManager.h"
#include "UI/TPSUIManager.h"
UTPSSystemManager* UTPSSystemManager::Instance = nullptr;

UTPSSystemManager::UTPSSystemManager(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	Instance = this;
}

UTPSSystemManager* UTPSSystemManager::CreateInstance()
{
	if (!Instance)
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

void UTPSSystemManager::Initialize()
{
	InitializeUIManager();
}

void UTPSSystemManager::Deinitialize()
{
	DeinitializeUIManager();
}

void UTPSSystemManager::Tick(float DeltaTime)
{
}

void UTPSSystemManager::PostTick(float DeltaTime)
{
}

UTPSUIManager* UTPSSystemManager::GetUIManager() const
{
	return UIManager;
}

void UTPSSystemManager::DeinitializeUIManager()
{
	if (UIManager != nullptr)
	{
		UIManager->Deinitialize();
		UIManager = nullptr;
	}
}

UWorld* UTPSSystemManager::GetWorld() const
{
	return World;
}

void UTPSSystemManager::SetWorld(UWorld* InWorld)
{
	if (World != InWorld)
	{
		World = InWorld;
	}
}

void UTPSSystemManager::InitializeUIManager()
{
	if (UIManager == nullptr)
	{
		UIManager = NewObject<UTPSUIManager>();
	}

	UIManager->Initialize();
}
