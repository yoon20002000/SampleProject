
#include "YGameEngine.h"
#include "YSystemManager.h"


void UYGameEngine::Init(class IEngineLoop* InEngineLoop)
{
	bClientInitialized = false;

	FWorldDelegates::OnPreWorldInitialization.AddStatic(OnPreWorldInitialization);
	FWorldDelegates::OnPostWorldInitialization.AddStatic(OnPostWorldInitialization);
	FWorldDelegates::OnWorldCleanup.AddStatic(OnWorldCleanup);

	SystemManager = UYSystemManager::CreateInstance();

	Super::Init(InEngineLoop);
}

void UYGameEngine::Start()
{

	Super::Start();
}

void UYGameEngine::PreExit()
{
	ShutDownClient();

	Super::PreExit();

	UYSystemManager::DestoryInstance();
	SystemManager = nullptr;
}

void UYGameEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	if (!bClientInitialized)
	{
		InitializeClient();
	}

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

void UYGameEngine::InitializeClient()
{
	if (bClientInitialized)
	{
		return;
	}

	bClientInitialized = true;
}

void UYGameEngine::ShutDownClient()
{
	if (bClientInitialized && SystemManager)
	{
		bClientInitialized = false;
	}
}

bool UYGameEngine::LoadMap(FWorldContext& WorldContext, FURL URL, class UPendingNetGame* Pending, FString& Error)
{
	if (!bClientInitialized)
	{
		// stop all event and unload all media
		//UAkGameplayStatics::OnMapUnload();
	}

	return Super::LoadMap(WorldContext, URL, Pending, Error);
}


void UYGameEngine::OnPreWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues IVS)
{
}

void UYGameEngine::OnPostWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues IVS)
{
}

void UYGameEngine::OnWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResources)
{

}


bool UYGameEngine::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return Super::Exec(InWorld, Cmd, Ar);
}

