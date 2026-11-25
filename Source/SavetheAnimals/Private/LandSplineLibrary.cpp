#include "LandSplineLibrary.h"
#include "LandscapeSplinesComponent.h"
#include "LandscapeSplineControlPoint.h"
#include "LandscapeSplineSegment.h"

namespace
{
	// Conn がつながっている区間の、反対側の制御点を返す
	ULandscapeSplineControlPoint* OtherEnd(const FLandscapeSplineConnection& Conn)
	{
		if (!Conn.Segment)
		{
			return nullptr;
		}
		return Conn.Segment->Connections[1 - Conn.End].ControlPoint;
	}
}

void ULandSplineLibrary::GetControlPointLocations(
	ULandscapeSplinesComponent* Splines,
	TArray<FVector>& OutWorldLocations,
	TArray<float>& OutWidths)
{
	OutWorldLocations.Reset();
	OutWidths.Reset();

	if (!Splines)
	{
		return;
	}

	const FTransform& ToWorld = Splines->GetComponentTransform();

	for (ULandscapeSplineControlPoint* CP : Splines->GetControlPoints())
	{
		if (!CP)
		{
			continue;
		}
		OutWorldLocations.Add(ToWorld.TransformPosition(CP->Location));
		OutWidths.Add(CP->Width);
	}
}

int32 ULandSplineLibrary::GetOrderedControlPointLocations(
	ULandscapeSplinesComponent* Splines,
	int32 ChainIndex,
	TArray<FVector>& OutWorldLocations)
{
	OutWorldLocations.Reset();
	if (!Splines)
	{
		return 0;
	}

	const FTransform& ToWorld = Splines->GetComponentTransform();
	const TArray<ULandscapeSplineControlPoint*> CPs = Splines->GetControlPoints();

	TSet<ULandscapeSplineControlPoint*> Visited;
	TArray<TArray<ULandscapeSplineControlPoint*>> Chains;

	auto Walk = [&](ULandscapeSplineControlPoint* Start)
		{
			TArray<ULandscapeSplineControlPoint*> Chain;
			ULandscapeSplineControlPoint* Cur = Start;
			while (Cur && !Visited.Contains(Cur))
			{
				Visited.Add(Cur);
				Chain.Add(Cur);

				ULandscapeSplineControlPoint* Next = nullptr;
				for (const FLandscapeSplineConnection& Conn : Cur->ConnectedSegments)
				{
					ULandscapeSplineSegment* Seg = Conn.Segment;
					if (!Seg)
					{
						continue;
					}
					ULandscapeSplineControlPoint* Other = Seg->Connections[1 - Conn.End].ControlPoint;
					if (Other && !Visited.Contains(Other))
					{
						Next = Other;
						break;
					}
				}
				Cur = Next;
			}
			Chains.Add(MoveTemp(Chain));
		};

	for (ULandscapeSplineControlPoint* CP : CPs)
	{
		if (CP && !Visited.Contains(CP) && CP->ConnectedSegments.Num() == 1)
		{
			Walk(CP);
		}
	}
	for (ULandscapeSplineControlPoint* CP : CPs)
	{
		if (CP && !Visited.Contains(CP))
		{
			Walk(CP);
		}
	}

	if (Chains.IsValidIndex(ChainIndex))
	{
		for (ULandscapeSplineControlPoint* CP : Chains[ChainIndex])
		{
			OutWorldLocations.Add(ToWorld.TransformPosition(CP->Location));
		}
	}
	return Chains.Num();
}

int32 ULandSplineLibrary::GetChainLocations(
	ULandscapeSplinesComponent* Splines,
	int32 ChainIndex,
	bool bReverse,
	TArray<FVector>& OutWorldLocations)
{
	OutWorldLocations.Reset();
	if (!Splines)
	{
		return 0;
	}

	const FTransform& ToWorld = Splines->GetComponentTransform();
	const TArray<ULandscapeSplineControlPoint*> CPs = Splines->GetControlPoints();

	TSet<ULandscapeSplineSegment*> UsedSegments;
	TArray<TArray<ULandscapeSplineControlPoint*>> Chains;

	// StartCP から FirstConn の区間へ進み、端の点か分岐点に着くまで歩く
	auto WalkFrom = [&](ULandscapeSplineControlPoint* StartCP, const FLandscapeSplineConnection& FirstConn)
		{
			TArray<ULandscapeSplineControlPoint*> Chain;
			Chain.Add(StartCP);

			UsedSegments.Add(FirstConn.Segment);
			ULandscapeSplineSegment* CameFrom = FirstConn.Segment;
			ULandscapeSplineControlPoint* Cur = OtherEnd(FirstConn);

			int32 Guard = 0;
			while (Cur && Guard++ < 100000)
			{
				if (Cur == StartCP)
				{
					break; // 輪になって戻ってきた(最初の点は重ねて追加しない)
				}
				Chain.Add(Cur);

				if (Cur->ConnectedSegments.Num() != 2)
				{
					break; // 端の点、または分岐点に着いた
				}

				ULandscapeSplineSegment* NextSeg = nullptr;
				ULandscapeSplineControlPoint* NextCP = nullptr;
				for (const FLandscapeSplineConnection& Conn : Cur->ConnectedSegments)
				{
					if (Conn.Segment && Conn.Segment != CameFrom && !UsedSegments.Contains(Conn.Segment))
					{
						NextSeg = Conn.Segment;
						NextCP = OtherEnd(Conn);
						break;
					}
				}
				if (!NextSeg)
				{
					break;
				}
				UsedSegments.Add(NextSeg);
				CameFrom = NextSeg;
				Cur = NextCP;
			}
			Chains.Add(MoveTemp(Chain));
		};

	// 1) 端の点・分岐点(つながる区間が2本ではない点)から歩く
	for (ULandscapeSplineControlPoint* CP : CPs)
	{
		if (!CP || CP->ConnectedSegments.Num() < 1 || CP->ConnectedSegments.Num() == 2)
		{
			continue;
		}
		for (const FLandscapeSplineConnection& Conn : CP->ConnectedSegments)
		{
			if (Conn.Segment && !UsedSegments.Contains(Conn.Segment))
			{
				WalkFrom(CP, Conn);
			}
		}
	}

	// 2) 端も分岐もない、輪になった道
	for (ULandscapeSplineControlPoint* CP : CPs)
	{
		if (!CP)
		{
			continue;
		}
		for (const FLandscapeSplineConnection& Conn : CP->ConnectedSegments)
		{
			if (Conn.Segment && !UsedSegments.Contains(Conn.Segment))
			{
				WalkFrom(CP, Conn);
				break;
			}
		}
	}

	if (Chains.IsValidIndex(ChainIndex))
	{
		for (ULandscapeSplineControlPoint* CP : Chains[ChainIndex])
		{
			OutWorldLocations.Add(ToWorld.TransformPosition(CP->Location));
		}
		if (bReverse)
		{
			for (int32 i = 0, j = OutWorldLocations.Num() - 1; i < j; ++i, --j)
			{
				OutWorldLocations.Swap(i, j);
			}
		}
	}
	return Chains.Num();
}

int32 ULandSplineLibrary::GetJunctionLocations(
	ULandscapeSplinesComponent* Splines,
	TArray<FVector>& OutWorldLocations,
	TArray<int32>& OutBranchCounts)
{
	OutWorldLocations.Reset();
	OutBranchCounts.Reset();
	if (!Splines)
	{
		return 0;
	}

	const FTransform& ToWorld = Splines->GetComponentTransform();
	for (ULandscapeSplineControlPoint* CP : Splines->GetControlPoints())
	{
		if (CP && CP->ConnectedSegments.Num() >= 3)
		{
			OutWorldLocations.Add(ToWorld.TransformPosition(CP->Location));
			OutBranchCounts.Add(CP->ConnectedSegments.Num());
		}
	}
	return OutWorldLocations.Num();
}