#pragma once

#include "CoreMinimal.h"
#include "TPSSystemManager.generated.h"


class UTPSUIManager;
class UTPSGameManager;

UCLASS()
class SAMPLEPROJECT_API UTPSSystemManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	static UTPSSystemManager* CreateInstance();
	static void DestoryInstance();
	static UTPSSystemManager* Get();

	void Initialize();
	void Deinitialize();
	void Tick(float DeltaTime);
	void PostTick(float DeltaTime);

	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UTPSUIManager* GetUIManager() const;
	
	void DeinitializeUIManager();

	virtual UWorld* GetWorld() const override;
	void SetWorld(UWorld* InWorld);

	UTPSGameManager* GetGameManager() const;
private:
	void InitializeUIManager();
	
private:
	static UTPSSystemManager* Instance;
	
	UPROPERTY(Transient)
	TObjectPtr<UTPSUIManager> UIManager;

	UPROPERTY(Transient)
	TObjectPtr<UTPSGameManager> GameManager;

	UPROPERTY()
	TObjectPtr<UWorld> World;
};
