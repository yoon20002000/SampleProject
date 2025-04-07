#pragma once

#include "Engine/DataTable.h"
#include "TPSBalanceData.generated.h"

UENUM(BlueprintType)
enum class ETPSBalanceStatus : uint8
{
	Easy = 0,
	Normal = 1 << 0,
	Hell = 1 << 1,
};

USTRUCT(BlueprintType)
struct FTPSBalanceData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FTPSBalanceData() : Level(1), SpawnAICount(5), SpawnPeriod(5)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnAICount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnPeriod;

	const static FName& GetBalanceStatusName(ETPSBalanceStatus status)
	{
		const static FName EasyName = "Easy";
		const static FName NormalName = "Normal";
		const static FName HellName = "Hell";

		switch (status)
		{
		case ETPSBalanceStatus::Easy:
		default:
			{
				return EasyName;
			}
		case ETPSBalanceStatus::Normal:
			{
				return NormalName;
			}
		case ETPSBalanceStatus::Hell:
			{
				return HellName;
			}
		}
	}
};
