// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GameDataAsset.generated.h"

class ATPSCharacter;

USTRUCT()
struct FCharacterAssetInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FString AssetName;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ATPSCharacter> Character;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AController> Controller;

	static FCharacterAssetInfo Invalid;
};

USTRUCT()
struct FGameTableInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FName TableName;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDataTable> DataTable;

	static FGameTableInfo Invalid;
};
/**
 * 
 */
UCLASS()
class UGameDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	const FCharacterAssetInfo& GetCharacterData(const FName& InName) const;
	const FGameTableInfo& GetGameTableData(const FName& InName) const;
private:	
	UPROPERTY(EditAnywhere)
	TArray<FCharacterAssetInfo> CharacterAssets;
	UPROPERTY(EditAnywhere)
	TArray<FGameTableInfo> DataTables;
};
