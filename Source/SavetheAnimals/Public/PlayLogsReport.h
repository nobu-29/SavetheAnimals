#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayLog.h"

#include "PlayLogsReport.generated.h"

UCLASS()
class SAVETHEANIMALS_API UPlayLogsReport
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    static bool ExportLogsToJson(
        const TArray<FPlayLog>& Logs,
        FString FileName);
};