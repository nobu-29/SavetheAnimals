#pragma once

#include "CoreMinimal.h"
#include "PlayerReport.generated.h"

USTRUCT(BlueprintType)
struct FPlayerReport
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DeathCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnemyDeaths = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FallDeaths = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TrapDeaths = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RetryCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HintUseCount = 0;
};