#pragma once

#include "CoreMinimal.h"
#include "TPSSystemManager.generated.h"


UCLASS()
class SAMPLEPROJECT_API UTPSSystemManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	static UTPSSystemManager* CreateInstance();
	static void DestoryInstance();
	static UTPSSystemManager* Get();

	void Tick(float DeltaTime);
	void PostTick(float DeltaTime);
	
private:
	static UTPSSystemManager* Instance;

};
