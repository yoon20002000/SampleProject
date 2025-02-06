#include "GameDataAsset.h"

FCharacterAssetInfo FCharacterAssetInfo::Invalid;

const FCharacterAssetInfo& UGameDataAsset::GetCharacterData(const FString& InName) const
{
	for (int32 i = 0; i < CharacterAssets.Num(); i++)
	{
		if (CharacterAssets[i].AssetName == InName)
		{
			return CharacterAssets[i];
		}
	}

	return FCharacterAssetInfo::Invalid;
}

FGameTableInfo FGameTableInfo::Invalid;

const FGameTableInfo& UGameDataAsset::GetGameTableData(const FString& InName) const
{
	for (const FGameTableInfo& TableInfo : DataTables)
	{
		if (TableInfo.TableName.Equals(InName) == true)
		{
			return TableInfo;
		}
	}
	return FGameTableInfo::Invalid;
}
