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
    int32 LoadKillCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PoisonCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DiseaseCount = 0;

};