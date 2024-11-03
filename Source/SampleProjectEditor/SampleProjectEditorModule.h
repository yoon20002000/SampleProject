
#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(YEditor, All, All)

class AUPCameraActor;
class UUPAct;

class FSampleProjectEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FSampleProjectEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FSampleProjectEditorModule >("SampleProjectEditor");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("SampleProjectEditor");
	}

};
