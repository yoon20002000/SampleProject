#pragma once

#include "CoreMinimal.h"
#include "TPSSystemManager.generated.h"


class UTPSUIManager;

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

	UTPSUIManager* GetUIManager() const;
	
	void DeinitializeUIManager();

	virtual UWorld* GetWorld() const override;
	void SetWorld(UWorld* InWorld);
private:
	void InitializeUIManager();
	
private:
	static UTPSSystemManager* Instance;
	
	UPROPERTY(Transient)
	TObjectPtr<UTPSUIManager> UIManager;

	UPROPERTY()
	TObjectPtr<UWorld> World;
};
