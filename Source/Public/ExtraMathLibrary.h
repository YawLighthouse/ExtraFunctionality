#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExtraMathLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExtraMathLibrary, Log, All);

UCLASS()
class EXTRAFUNCTIONALITY_API UExtraMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#pragma region Integer Math

	/** 
	* Handles looping InValue between zero and LoopAt only really works with positive values.
	* EX: InValue = (1 + -2)[so really it inputted -1], LoopAt = 5, return = 4.
	* EX: InValue = (4 + 2)[so really it inputted 6], LoopAt = 5, return = 1.
	* @param LoopAt If InValue is equal to this value then it will loop back to zero so if your using a last index in an array its recommended to add 1 to have it loop after it hits the last index.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Integer", 
		meta = (DisplayName = "Loop To Range (int)"))
		static int LoopToRange_Int(int InValue, int LoopAt);

	/** Generate a random number between X(Min) and Y(Max) */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Integer", 
		meta = (DisplayName = "Random Integer In Range (int point)"))
		static int RandomIntegerInRange_IntPoint(FIntPoint InIntPoint);

	/**
	* Handles sorting an array of integers, the direction is based on IsReversed
	* @param IsReversed If true will sort from lowest to highest where Lowest is index 0 and Highest is last index. False is the inverse of what true outputs.
	* @return Returns a copy of InArray but sorted so its nondestructive.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Integer", 
		meta = (DisplayName = "Sort (int)"))
		static TArray<int> Sort_Int(TArray<int> InArray, bool IsReversed);

	/**
	* Returns a negated copy of InValue, if already negative then turns it into a positive value.
	* To ensure the return value is always negative then make sure the InValue is positive using the Absolute function.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Integer",
		meta = (DisplayName = "Negative (integer)", CompactNodeTitle = "-i"))
		static int Negate_Int(int InValue);

#pragma endregion

#pragma region Float Math

	/** Performs the operation 1 - InValue, meant to be a shorthand function. */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Float",
		meta = (CompactNodeTitle = "1-", Keywords = "-1"))
	static float OneMinus(float InValue);

	/**
	* Handles looping InValue between zero and LoopAt only really works with positive values.
	* EX: InValue = (1 + -2)[so really it inputted -1], LoopAt = 5, return = 4.
	* EX: InValue = (4 + 2)[so really it inputted 6], LoopAt = 5, return = 1.
	* @param LoopAt If InValue is equal to this value then it will loop back to zero so if your using a last index in an array its recommended to add 1 to have it loop after it hits the last index.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Float", 
		meta = (DisplayName = "Loop To Range (float)"))
		static float LoopToRange_Float(float InValue, float LoopAt);

	/** Generate a random number between X(Min) and Y(Max) */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Float", 
		meta = (DisplayName = "Random Float In Range (vector2d)"))
		static float RandomFloatInRange_Vector2D(FVector2D InVector2D);

	/**
	* Handles sorting an array of floats, the direction is based on IsReversed
	* @param IsReversed If true will sort from lowest to highest where Lowest is index 0 and Highest is last index. False is the inverse of what true outputs.
	* @return Returns a copy of InArray but sorted so its nondestructive.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Float", 
		meta = (DisplayName = "Sort (float)"))
		static TArray<float> Sort_Float(TArray<float> InArray, bool IsReversed);

	/** 
	* Returns a negated copy of InValue, if already negative then turns it into a positive value. 
	* To ensure the return value is always negative then make sure the InValue is positive using the Absolute function.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Float",
		meta = (DisplayName = "Negative (float)", CompactNodeTitle = "-f"))
		static float Negate_Float(float InValue);

#pragma endregion

#pragma region Vector2D Stuff

	/** Handles rounding each component of the 2D vector and returning that rounded 2D vector. */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector2D")
	static FVector2D Round2DVector(FVector2D Value);

	/** Returns true if Value is in range of Min and Max. Compares each component of vector(so if X is in range of Min.X and Max.X and same for Y). */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector2D")
	static bool InRange(FVector2D Value, FVector2D Min, FVector2D Max);

	/** Returns the center point between 2 vector2D points */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector2D")
	static FVector2D GetCenterBetweenPoints2D(FVector2D V1, FVector2D V2);

	/** Returns the lowest components of A and B. */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector2D", 
		meta = (DisplayName = "Min (vector2d)", CompactNodeTitle = "MIN"))
	static FVector2D Vector2D_Min(FVector2D A, FVector2D B);

	/** Returns the highest components of A and B. */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector2D", 
		meta = (DisplayName = "Max (vector2d)", CompactNodeTitle = "MAX"))
	static FVector2D Vector2D_Max(FVector2D A, FVector2D B);

	/** Clamps each component of Value between Min and Max(so clamps X to Min.X and Max.X, same for Y). */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector2D")
	static FVector2D ClampVector2D(FVector2D Value, FVector2D Min, FVector2D Max);

#pragma endregion

#pragma region Vector Stuff

	/** Handles rounding each component of the vector and returning that rounded vector. */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector")
	static FVector RoundVector(FVector Value);

	/** Returns the center point between 2 vector points */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector")
	static FVector GetCenterBetweenPoints(FVector V1, FVector V2);

	/** Clamps all components of Vector to Min and Max(so clamps X to Min.X and Max.X and same for Y & Z). */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Vector")
	static FVector ClampToBounds(FVector Point, FVector Min, FVector Max);

#pragma endregion

#pragma region Rotator Stuff

	/** A-B */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Rotator", 
		meta = (DisplayName = "Difference (Rotator)", ScriptMethod = "Diff", Keywords = "Delta",
			CompactNodeTitle = "A - B"))
		static FRotator RotatorDifference(FRotator A, FRotator B);

	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Rotator")
	static FRotator GridSnapRotation(FRotator Rotator, float GridDeg);

#pragma endregion

};


