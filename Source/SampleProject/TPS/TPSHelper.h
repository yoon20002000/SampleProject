#pragma once

#include "CoreMinimal.h"

class ATPSPlayer;
class UTPSGameInstance;
class ATPSGameMode;

class TPSHelper
{

private:
	static TWeakObjectPtr<UTPSGameInstance> GameInstance;

public:

	static void SetGameInstance(UTPSGameInstance* gameInstance);

	static UWorld* GetWorld();
	static UTPSGameInstance* GetGameInstance();
	static APlayerController* GetPlayerController(UWorld* InWorld = nullptr);
	static ATPSPlayer* GetPlayerOrNullptr();
	static ULocalPlayer* GetLocalPlayer(UWorld* InWorld = nullptr);
	static APlayerCameraManager* GetPlayerCameraManager(UWorld* InWorld = nullptr);
	static APlayerController* GetFirstLocalPlayerController(UWorld* InWorld);
};
