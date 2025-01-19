#include "TPSSystemManager.h"

#include "TPSGameManager.h"
#include "TPSHelper.h"
#include "Game/TPSGameStateManager.h"
#include "System/TPSNameplateManager.h"
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

void UTPSSystemManager::DestroyInstance()
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
	GameManager = NewObject<UTPSGameManager>();
	GameStateManager = NewObject<UTPSGameStateManager>();
	NamePlateManager = NewObject<UTPSNameplateManager>();
}

void UTPSSystemManager::Uninitialize()
{
	DeinitializeUIManager();
	
	if (UIManager != nullptr)
	{
		UIManager->ConditionalBeginDestroy();
	}
	if (GameManager != nullptr)
	{
		GameManager->ConditionalBeginDestroy();	
	}
	if (GameStateManager != nullptr)
	{
		GameStateManager->ConditionalBeginDestroy();	
	}
	if (NamePlateManager != nullptr)
	{
		NamePlateManager->ConditionalBeginDestroy();	
	}
}

void UTPSSystemManager::Tick(float DeltaTime)
{
}

void UTPSSystemManager::PostTick(float DeltaTime)
{
}

void UTPSSystemManager::BeginPlay()
{
	GameManager->BeginPlay();
	UIManager->BeginPlay();
	GameStateManager->BeginPlay();
	NamePlateManager->BeginPlay();
}

void UTPSSystemManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UIManager->EndPlay(EndPlayReason);
	GameManager->EndPlay(EndPlayReason);
	NamePlateManager->EndPlay(EndPlayReason);
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

UTPSGameManager* UTPSSystemManager::GetGameManager() const
{
	return GameManager;
}

UTPSGameStateManager* UTPSSystemManager::GetGameStateManager() const
{
	return GameStateManager;
}

UTPSNameplateManager* UTPSSystemManager::GetNameplateManager() const
{
	return NamePlateManager;
}

void UTPSSystemManager::SetGameState(const EGameplayState InGameplayState)
{
	if (GameStateManager == nullptr)
	{
		return;
	}
	GameStateManager->SetGameplayState(InGameplayState);
}

void UTPSSystemManager::QuitGame() const
{
	if (APlayerController* PlayerController = TPSHelper::GetPlayerController(World))
	{
		PlayerController->ConsoleCommand("quit");
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
