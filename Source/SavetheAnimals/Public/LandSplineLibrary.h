#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LandSplineLibrary.generated.h"

class ULandscapeSplinesComponent;

UCLASS()
class SAVETHEANIMALS_API ULandSplineLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "LandSpline")
	static void GetControlPointLocations(
		ULandscapeSplinesComponent* Splines,
		TArray<FVector>& OutWorldLocations,
		TArray<float>& OutWidths);

	UFUNCTION(BlueprintCallable, Category = "LandSpline")
	static int32 GetOrderedControlPointLocations(
		ULandscapeSplinesComponent* Splines,
		int32 ChainIndex,
		TArray<FVector>& OutWorldLocations);

	// 追加: 端の点・分岐点で区切った「1本の道」を返す。戻り値は道の本数
	UFUNCTION(BlueprintCallable, Category = "LandSpline")
	static int32 GetChainLocations(
		ULandscapeSplinesComponent* Splines,
		int32 ChainIndex,
		bool bReverse,
		TArray<FVector>& OutWorldLocations);

	// 追加: 3本以上つながる点(分岐点)の位置と、つながっている道の本数
	UFUNCTION(BlueprintCallable, Category = "LandSpline")
	static int32 GetJunctionLocations(
		ULandscapeSplinesComponent* Splines,
		TArray<FVector>& OutWorldLocations,
		TArray<int32>& OutBranchCounts);
};