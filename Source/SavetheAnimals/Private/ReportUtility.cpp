// Fill out your copyright notice in the Description page of Project Settings.


#include "ReportUtility.h"

#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "PlayerReport.h"

void UReportUtility::ExportReportJson(
	const FPlayerReport& Report)
{
	FString JsonString;

	FJsonObjectConverter::
		UStructToJsonObjectString(
			Report,
			JsonString
		);

	FString SavePath =
		FPaths::ProjectSavedDir() +
		TEXT("PlayerReport.json");

	FFileHelper::SaveStringToFile(
		JsonString,
		*SavePath
	);
}