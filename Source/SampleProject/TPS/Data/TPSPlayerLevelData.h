#pragma once

#include "TPSBalanceData.h"
#include "TPSPlayerLevelData.generated.h"

enum class ETPSBalanceStatus : uint8;

USTRUCT(BlueprintType)
struct FTPSPlayerLevelData : public FTableRowBase
{
	GENERATED_BODY()
	FTPSPlayerLevelData() : KillCount(0), BalanceType(ETPSBalanceStatus::Easy)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KillCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETPSBalanceStatus BalanceType;
};
