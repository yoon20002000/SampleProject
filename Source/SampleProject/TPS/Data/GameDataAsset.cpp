#include "GameDataAsset.h"

FCharacterAssetInfo FCharacterAssetInfo::Invalid = FCharacterAssetInfo{TEXT("Invalid"), nullptr, nullptr};

const FCharacterAssetInfo& UGameDataAsset::GetCharacterData(const FName& InName) const
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

const FGameTableInfo& UGameDataAsset::GetGameTableData(const FName& InName) const
{
	for (const FGameTableInfo& TableInfo : DataTables)
	{
		if (TableInfo.TableName == InName)
		{
			return TableInfo;
		}
	}
	return FGameTableInfo::Invalid;
}
