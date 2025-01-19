#pragma once

#include "TPSSystemManager.generated.h"

class UTPSNameplateManager;
enum class EGameplayState : uint8;
class UTPSGameStateManager;
class UTPSUIManager;
class UTPSGameManager;

UCLASS()
class SAMPLEPROJECT_API UTPSSystemManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	static UTPSSystemManager* CreateInstance();
	static void DestroyInstance();
	static UTPSSystemManager* Get();

	void Initialize();
	void Uninitialize();
	void Tick(float DeltaTime);
	void PostTick(float DeltaTime);

	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UTPSUIManager* GetUIManager() const;

	void DeinitializeUIManager();

	virtual UWorld* GetWorld() const override;
	void SetWorld(UWorld* InWorld);

	UTPSGameManager* GetGameManager() const;
	UTPSGameStateManager* GetGameStateManager() const;
	UTPSNameplateManager* GetNameplateManager() const;
	void SetGameState(const EGameplayState InGameplayState);
	void QuitGame() const;
	
private:
	void InitializeUIManager();

private:
	static UTPSSystemManager* Instance;

	UPROPERTY(Transient)
	TObjectPtr<UTPSUIManager> UIManager;

	UPROPERTY(Transient)
	TObjectPtr<UTPSGameManager> GameManager;

	UPROPERTY(Transient)
	TObjectPtr<UTPSGameStateManager> GameStateManager;
	
	UPROPERTY(Transient)
	TObjectPtr<UTPSNameplateManager> NamePlateManager;
	
	UPROPERTY()
	TObjectPtr<UWorld> World;
};
