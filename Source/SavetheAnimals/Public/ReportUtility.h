// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayerReport.h"
#include "ReportUtility.generated.h"

/**
 * 
 */
UCLASS()
class SAVETHEANIMALS_API UReportUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public :
	UFUNCTION(BlueprintCallable, Category= "Report")
	static void ExportReportJson(
		const FPlayerReport& Report);
};
