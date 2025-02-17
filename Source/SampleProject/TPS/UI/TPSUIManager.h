#pragma once

#include "CommonUIExtensions.h"
#include "GameplayTagsManager.h"
#include "Data/UIDataAsset.h"
#include "CommonActivatableWidget.h"
#include "TPSUIManager.generated.h"

class UUIDataAsset;
class UTPSGameUIPolicy;
class UTPSCommonLocalPlayer;
/**
 * 
 */

UENUM(Blueprintable)
enum class EUILayerType : uint8
{
	NONE = 0,
	GameLayer,
	GameMenu,
	Menu,
	Modal,
};

UCLASS(config = Game)
class SAMPLEPROJECT_API UTPSUIManager : public UObject
{
	GENERATED_BODY()
public:
	virtual void Initialize();
	virtual void Deinitialize();

	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type EndPlayReason);
	
	const UTPSGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UTPSGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }
	
	virtual void NotifyPlayerAdded(UTPSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UTPSCommonLocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UTPSCommonLocalPlayer* LocalPlayer);

	template<typename T = UCommonActivatableWidget>
	T* LoadUI(const FString& UIName, EUILayerType LayerType = EUILayerType::GameLayer)
	{
		if (UIDataAsset == nullptr)
		{
			return nullptr;
		}

		UUserWidget* LoadWidget = UIDataAsset->LoadUserWidget(UIName);

		if (LoadWidget != nullptr)
		{
			FString LayerName = GetLayerNameByLayerType(LayerType);

			const TObjectPtr<UCommonActivatableWidget> LoadUIPtr = UCommonUIExtensions::PushContentToLayer(
				UGameplayTagsManager::Get().RequestGameplayTag(*LayerName),
				CastChecked<T>(LoadWidget)->GetClass());
			LoadedUIs.Add(LoadUIPtr);
			return Cast<T>(LoadUIPtr);
		}
		return nullptr;
	}
	void RemoveUI(UCommonActivatableWidget* InRemoveWidget);
	void RemoveAllUIs();
	FString GetLayerNameByLayerType(const EUILayerType InLayerType);
	
protected:
	void SwitchToPolicy(UTPSGameUIPolicy* InPolicy);
private:
	UPROPERTY(Transient)
	TObjectPtr<UTPSGameUIPolicy> CurrentPolicy = nullptr;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UTPSGameUIPolicy> DefaultUIPolicyClass;

	UPROPERTY(Transient)
	TObjectPtr<UUIDataAsset> UIDataAsset = nullptr;

	UPROPERTY(config, EditAnywhere)
	TSoftObjectPtr<UUIDataAsset> UIDataAssetClass;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UCommonActivatableWidget>> LoadedUIs;
};
