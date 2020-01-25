

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
	LoopAt = FMath::Max(LoopAt, 1);

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

int UExtraMathLibrary::Negate_Int(int InValue)
{
	return -InValue;
}

float UExtraMathLibrary::OneMinus(float InValue)
{
	return (1.0f - InValue);
}

float UExtraMathLibrary::LoopToRange_Float(float InValue, float LoopAt)
{
	LoopAt = FMath::Max(LoopAt, 1.0f);

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

float UExtraMathLibrary::Negate_Float(float InValue)
{
	return -InValue;
}

FVector2D UExtraMathLibrary::Round2DVector(FVector2D Value)
{
	return FVector2D(FMath::RoundToFloat(Value.X),
		FMath::RoundToFloat(Value.Y));
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

FVector UExtraMathLibrary::RoundVector(FVector Value)
{
	return FVector(FMath::RoundToFloat(Value.X),
		FMath::RoundToFloat(Value.Y),
		FMath::RoundToFloat(Value.Z));
}

FVector UExtraMathLibrary::GetCenterBetweenPoints(FVector V1, FVector V2)
{
	return ((V1 + V2) * 0.5f);
}

FVector UExtraMathLibrary::ClampToBounds(FVector Point, FVector Min, FVector Max)
{
	return Point.BoundToBox(Min, Max);
}

FRotator UExtraMathLibrary::RotatorDifference(FRotator A, FRotator B)
{
	FRotator Diff = (A - B);
	return Diff;
}

FRotator UExtraMathLibrary::GridSnapRotation(FRotator Rotator, float GridDeg)
{
	if (FMath::IsNearlyZero(GridDeg))
	{
		return Rotator;
	}

	// We assume grid size values of max 90	
	const float gridRad = FMath::DegreesToRadians(GridDeg);
	FQuat quat = Rotator.Quaternion();

	FVector forward = quat.GetForwardVector();
	FVector right = quat.GetRightVector();

	// Forward
	// To get the forward vector, we snap the forward vector of the quaternion
	// to a linear representation of the rotation.
	FVector forwardResult;
	{
		// Bring the vector into linear space
		forwardResult = FVector(
			FMath::Asin(forward.X)
			, FMath::Asin(forward.Y)
			, FMath::Asin(forward.Z));

		// Snap in linear space
		forwardResult = FVector(
			FMath::GridSnap(forwardResult.X, gridRad)
			, FMath::GridSnap(forwardResult.Y, gridRad)
			, FMath::GridSnap(forwardResult.Z, gridRad));

		// Back to sine space
		forwardResult = FVector(
			FMath::Sin(forwardResult.X)
			, FMath::Sin(forwardResult.Y)
			, FMath::Sin(forwardResult.Z));

		// Ensure our forward vector is not Zero
		if (forwardResult.IsNearlyZero())
		{
			TArray<float> maxHelper = { FMath::Abs(forward.X), FMath::Abs(forward.Y), FMath::Abs(forward.Z) };
			int32 maxIndex = INDEX_NONE;
			FMath::Max(maxHelper, &maxIndex);
			forwardResult[maxIndex] = FMath::Sign(forward[maxIndex]);
		}

		// Snapping each component of the vector to the grid does not yet place the vector
		// in the rotation grid (when z != 0). We need to make a correction that also normalizes
		// the vector again.
		// E.g 45 deg rotated around the y axis and then 45deg rotated around the z axis.
		// Cause of the component snapping, all components of the vector are now 0.707 (sin space).
		// Only the z component is valid to have 0.707, X and Y must be adjusted.
		// The proper result must be FVector(0.5, 0.5, 0.707)
		float sizeXYTarget = FMath::Sqrt(1 - FMath::Square(forwardResult.Z));
		FVector2D vec2D = FVector2D(forwardResult);
		float size2d = vec2D.Size();
		if (FMath::IsNearlyZero(size2d))
		{
			vec2D *= 0;
		}
		else
		{
			vec2D *= sizeXYTarget / size2d;
		}

		forwardResult.Normalize();
	}

	// Right
	// To get the right vector we rotate in grid distance around the forward vector
	// and take the vector that is closest to the original right vector.
	FVector rightResult;
	{
		FVector rightTemp;
		if (forwardResult.Equals(FVector(0.f, 0.f, 1.f)))
		{
			rightTemp = FVector(0.f, 1.f, 0.f);
		}
		else if (forwardResult.Equals(FVector(0.f, 0.f, -1.f)))
		{
			rightTemp = FVector(0.f, -1.f, 0.f);
		}
		else
		{
			rightTemp = FVector(0.f, 0.f, 1.f) ^ forwardResult;
			rightTemp.Normalize();
		}
		FVector bestMatch = rightTemp;
		float distClosest = FVector::DistSquared(rightTemp, right);

		bool bInversed = false;
		bool bWasCloser = false;
		int32 rotMultiplier = 0;
		while (true)
		{
			rotMultiplier = rotMultiplier + (bInversed ? -1 : 1);
			FVector rightRotated = rightTemp.RotateAngleAxis(GridDeg * rotMultiplier, forwardResult);
			float dist = FVector::DistSquared(rightRotated, right);
			if (dist < distClosest || FMath::IsNearlyEqual(dist, distClosest, KINDA_SMALL_NUMBER))
			{
				bWasCloser = true;
				distClosest = dist;
				bestMatch = rightRotated;
			}
			else if (dist > distClosest)
			{
				// Getting further away from our target
				if (!bInversed)
				{
					// First time, inverse rotation
					bInversed = true;
				}
				else if (bWasCloser)
				{
					// Have been closest possible already and getting further away again: closest possible found
					break;
				}
			}
		}

		rightResult = bestMatch;
	}

	// Up
	FVector upResult;
	{
		upResult = forwardResult ^ rightResult;
		upResult.Normalize();
	}

	FRotator out = UKismetMathLibrary::MakeRotationFromAxes(forwardResult, rightResult, upResult);
	ensure(!out.ContainsNaN());
	return out;
}

