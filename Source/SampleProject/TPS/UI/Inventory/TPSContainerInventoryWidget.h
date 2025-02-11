// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TPSCommonActivatableWidget.h"
#include "TPSContainerInventoryWidget.generated.h"

class UTPSInventoryComponent;
class UTPSInventoryGridWidget;
/**
 * 
 */
UCLASS()
class SAMPLEPROJECT_API UTPSContainerInventoryWidget : public UTPSCommonActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual  void NativeDestruct() override;
	void Init(UTPSInventoryComponent* InventoryComp);
private:
	UPROPERTY(meta=(AllowPrivateAccess=true, BindWidget))
	TObjectPtr<UTPSInventoryGridWidget> PlayerInventoryGrid;
	UPROPERTY(meta=(AllowPrivateAccess=true, BindWidget))
	TObjectPtr<UTPSInventoryGridWidget> ContainerInventoryGrid;

	TWeakObjectPtr<UTPSInventoryComponent> PlayerInventoryComp;
	TWeakObjectPtr<UTPSInventoryComponent> ContainerInventoryComp;
};
