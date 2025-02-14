#pragma once

#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

struct FInventorySlot;
class UTPSSaveGame;
class UGameDataAsset;
class UTPSUIManager;
class ATPSGameMode;

/**
 * 
 */
UCLASS(Blueprintable)
class SAMPLEPROJECT_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;
	virtual void ReturnToMainMenu() override;
	virtual void Shutdown() override;
	virtual void SaveGameToSlot(UTPSSaveGame* SaveGameData);

	void SaveInventoryData(const TArray<FInventorySlot>& InventorySlots);
	const TArray<FInventorySlot>& GetInventoryData();
	UTPSSaveGame* GetSaveGameData() const;
public:
	const FString SaveData_Name = TEXT("TPSSaveData");
private:
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
	UPROPERTY()
	TObjectPtr<UTPSSaveGame> PlayerSaveData;
};
