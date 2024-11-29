// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "TPSActionComponent.generated.h"

class UTPSAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged,
                                             UTPSActionComponent*, OwningComp,
                                             UTPSAction*, Action);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SAMPLEPROJECT_API UTPSActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTPSActionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UTPSAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UTPSAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UTPSAction* GetAction(const TSubclassOf<UTPSAction> ActionClass) const;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FGameplayTag ActionName);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FGameplayTag ActionName);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FGameplayTag ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FGameplayTag ActionName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

protected:
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UTPSAction>> DefaultActions;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<TObjectPtr<UTPSAction>> Actions;
};
