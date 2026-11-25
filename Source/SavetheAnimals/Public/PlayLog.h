#pragma once

#include "CoreMinimal.h"
#include "PlayLog.generated.h"

USTRUCT(BlueprintType)
struct FPlayLog
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite)
    float DeathTime;

    UPROPERTY(BlueprintReadWrite)
    FString DeathReason;

};