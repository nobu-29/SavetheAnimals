#include "PlayLogsReport.h"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool UPlayLogsReport::ExportLogsToJson(
    const TArray<FPlayLog>& Logs,
    FString FileName)
{
    TArray<TSharedPtr<FJsonValue>> JsonLogs;

    for (const FPlayLog& Log : Logs)
    {
        TSharedPtr<FJsonObject> JsonObject =
            MakeShareable(new FJsonObject());

        JsonObject->SetNumberField(
            TEXT("DeathTime"),
            Log.DeathTime);

        JsonObject->SetStringField(
            TEXT("DeathReason"),
            Log.DeathReason);

        JsonLogs.Add(
            MakeShareable(
                new FJsonValueObject(JsonObject)));
    }

    TSharedPtr<FJsonObject> Root =
        MakeShareable(new FJsonObject());

    Root->SetArrayField(
        TEXT("Logs"),
        JsonLogs);

    FString JsonString;

    TSharedRef<TJsonWriter<>> Writer =
        TJsonWriterFactory<>::Create(
            &JsonString);

    FJsonSerializer::Serialize(
        Root.ToSharedRef(),
        Writer);

    FString SavePath =
        FPaths::ProjectSavedDir()
        + FileName;

    return FFileHelper::SaveStringToFile(
        JsonString,
        *SavePath);
}