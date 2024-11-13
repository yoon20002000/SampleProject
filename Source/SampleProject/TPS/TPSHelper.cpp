#include "TPSHelper.h"

#include "TPSSystemManager.h"
#include "Game/TPSGameInstance.h"
#include "Game/TPSGameMode.h"
#include "Kismet/GameplayStatics.h"

TWeakObjectPtr<UTPSGameInstance> TPSHelper::GameInstance = nullptr;

UWorld* TPSHelper::GetWorld()
{
	UWorld* World = UTPSSystemManager::Get()->GetWorld();
	if (World == nullptr && GEngine->GameViewport != nullptr)
	{
		World = GEngine->GameViewport->GetWorld();
	}

	return World;
}

UTPSGameInstance* TPSHelper::GetGameInstance()
{
	if (GameInstance.IsValid())
	{
		return GameInstance.Get();
	}
	return nullptr;
}


APlayerController* TPSHelper::GetPlayerController(UWorld* InWorld)
{
	if (InWorld != nullptr)
	{
		return UGameplayStatics::GetPlayerController(InWorld, 0);
	}
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

APlayerCameraManager* TPSHelper::GetPlayeCameraManager(UWorld* InWorld)
{
	APlayerController* PlayerController = GetPlayerController(InWorld);
	if (PlayerController != nullptr)
	{		
		return PlayerController->PlayerCameraManager;
	}	
	return nullptr;
}

APlayerController* TPSHelper::GetFirstLocalPlayerController(UWorld* InWorld)
{
	if (InWorld == nullptr)
	{
		return nullptr;
	}
	APlayerController* playerController = Cast<APlayerController>((GetWorld() && GEngine) ? GEngine->GetFirstLocalPlayerController(GetWorld()) : NULL);
	return playerController;
}
