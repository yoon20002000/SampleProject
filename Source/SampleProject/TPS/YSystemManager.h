#pragma once

#include "CoreMinimal.h"
#include "YSystemManager.generated.h"


UCLASS()
class SAMPLEPROJECT_API UYSystemManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	static UYSystemManager* CreateInstance();
	static void DestoryInstance();
	static UYSystemManager* Get();

	void Tick(float DeltaTime);
	void PostTick(float DeltaTime);
	
private:
	static UYSystemManager* Instance;

};
