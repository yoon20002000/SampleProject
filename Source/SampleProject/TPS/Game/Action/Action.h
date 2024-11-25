// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"

#include "Action.generated.h"

class UWorld;
class UActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	bool bIsRunning;

	UPROPERTY()
	TObjectPtr<AActor> Instigator;

	FActionRepData() : bIsRunning(false)
	{
	}
};

/**
 * 
 */
UCLASS(Blueprintable)
class SAMPLEPROJECT_API UAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintPure)
	FGameplayTag GetActivationTag() const;

	UFUNCTION(BlueprintPure)
	bool IsAutoStart() const;

	void Initialize(UActionComponent* NewActionComp);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;
	virtual bool IsSupportedForNetworking() const override;
protected:
	
	UFUNCTION()
	void OnRep_RepData();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	TObjectPtr<UActionComponent> ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UPROPERTY(Replicated)
	float TimeStared;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTag ActionTag;
};
