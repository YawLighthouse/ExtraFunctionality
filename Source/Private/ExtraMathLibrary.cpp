

#include "ExtraMathLibrary.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GenericPlatformMisc.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Console.h"

DEFINE_LOG_CATEGORY(LogExtraMathLibrary);

int UExtraMathLibrary::LoopToRange_Int(int InValue, int LoopAt)
{
	if (FMath::Abs(InValue) < FMath::Abs(LoopAt))
	{
		return (InValue < 0) ? 
			LoopToRange_Int(LoopAt + InValue, LoopAt)/*recursively will handle looping the value if its negative*/
			: InValue/* we already checked if it was positive and less than LoopAt so no need to make it abs */;
	}
		
	return (FMath::Abs(InValue) % FMath::Abs(LoopAt));
}

int UExtraMathLibrary::RandomIntegerInRange_IntPoint(FIntPoint InIntPoint)
{
	return FMath::RandRange(InIntPoint.X, InIntPoint.Y);
}

TArray<int> UExtraMathLibrary::Sort_Int(TArray<int> InArray, bool IsReversed)
{	
	// Using lambda version cause lambda's are fun
	if (IsReversed)
	{
		InArray.StableSort([](const int& A, const int& B)
		{
			return (A < B);
		});
	}
	else
	{
		InArray.StableSort([](const int& A, const int& B)
		{
			return (A > B);
		});
	}
	return InArray;
}

float UExtraMathLibrary::LoopToRange_Float(float InValue, float LoopAt)
{
	if (FMath::Abs(InValue) < FMath::Abs(LoopAt))
	{
		return (InValue < 0.0f) ?
			LoopToRange_Float(LoopAt + InValue, LoopAt)/*recursively will handle looping the value if its negative*/
			: InValue/* we already checked if it was positive and less than LoopAt so no need to make it abs */;
	}

	return ((FMath::Abs(LoopAt) != 0.0f) ? FMath::Fmod(FMath::Abs(InValue), FMath::Abs(LoopAt)) : 0.0f);
}

float UExtraMathLibrary::RandomFloatInRange_Vector2D(FVector2D InVector2D)
{
	return FMath::RandRange(InVector2D.X, InVector2D.Y);
}

TArray<float> UExtraMathLibrary::Sort_Float(TArray<float> InArray, bool IsReversed)
{
	// Using lambda version cause lambda's are fun
	if (IsReversed)
	{
		InArray.StableSort([](const float& A, const float& B)
		{
			return (A < B);
		});
	}
	else
	{
		InArray.StableSort([](const float& A, const float& B)
		{
			return (A > B);
		});
	}
	return InArray;
}

bool UExtraMathLibrary::InRange(FVector2D Value, FVector2D Min, FVector2D Max)
{
	return (UKismetMathLibrary::InRange_FloatFloat(Value.X, Min.X, Max.X) && UKismetMathLibrary::InRange_FloatFloat(Value.Y, Min.Y, Max.Y));
}

FVector2D UExtraMathLibrary::GetCenterBetweenPoints2D(FVector2D V1, FVector2D V2)
{
	return ((V1 + V2) * 0.5f);
}

FVector2D UExtraMathLibrary::Vector2D_Min(FVector2D A, FVector2D B)
{
	return FVector2D((A.X < B.X) ? A.X : B.X, (A.Y < B.X) ? A.Y : B.Y);
}

FVector2D UExtraMathLibrary::Vector2D_Max(FVector2D A, FVector2D B)
{
	return FVector2D((A.X > B.X) ? A.X : B.X, (A.Y > B.X) ? A.Y : B.Y);
}

FVector2D UExtraMathLibrary::ClampVector2D(FVector2D Value, FVector2D Min, FVector2D Max)
{
	return FVector2D(FMath::Clamp(Value.X, Min.X, Max.X), FMath::Clamp(Value.Y, Min.Y, Max.Y));
}

FVector UExtraMathLibrary::GetCenterBetweenPoints(FVector V1, FVector V2)
{
	return ((V1 + V2) * 0.5f);
}

FVector UExtraMathLibrary::ClampToBounds(FVector Point, FVector Min, FVector Max)
{
	return Point.BoundToBox(Min, Max);
}

