
#include "TPSGameEngine.h"
#include "TPSSystemManager.h"


void UTPSGameEngine::Init(class IEngineLoop* InEngineLoop)
{
	bClientInitialized = false;

	FWorldDelegates::OnPreWorldInitialization.AddStatic(OnPreWorldInitialization);
	FWorldDelegates::OnPostWorldInitialization.AddStatic(OnPostWorldInitialization);
	FWorldDelegates::OnWorldCleanup.AddStatic(OnWorldCleanup);

	SystemManager = UTPSSystemManager::CreateInstance();

	SystemManager->Initialize();
	Super::Init(InEngineLoop);
}

void UTPSGameEngine::Start()
{

	Super::Start();
}

void UTPSGameEngine::PreExit()
{
	ShutDownClient();

	Super::PreExit();

	UTPSSystemManager::DestroyInstance();
	SystemManager = nullptr;
}

void UTPSGameEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	if (!bClientInitialized)
	{
		InitializeClient();
	}

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

void UTPSGameEngine::InitializeClient()
{
	if (bClientInitialized)
	{
		return;
	}

	bClientInitialized = true;
}

void UTPSGameEngine::ShutDownClient()
{
	if (bClientInitialized && SystemManager)
	{
		bClientInitialized = false;
	}
}

bool UTPSGameEngine::LoadMap(FWorldContext& WorldContext, FURL URL, class UPendingNetGame* Pending, FString& Error)
{
	if (!bClientInitialized)
	{
		// stop all event and unload all media
		//UAkGameplayStatics::OnMapUnload();
	}

	return Super::LoadMap(WorldContext, URL, Pending, Error);
}


void UTPSGameEngine::OnPreWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues IVS)
{
}

void UTPSGameEngine::OnPostWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues IVS)
{
}

void UTPSGameEngine::OnWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResources)
{

}


bool UTPSGameEngine::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return Super::Exec(InWorld, Cmd, Ar);
}

