#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h" 
#include "Components/SplineMeshComponent.h"
#include "EngineMinimal.h"
#include "Engine/EngineTypes.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Core/Public/Containers/EnumAsByte.h"
#include "ExtraDataTypes.generated.h"

class UStaticMesh;
class UMaterialInterface;

template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value, const bool bUseFullValueName=false)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!EnumPtr)
	{
		return FString("Invalid");
	}

	if (bUseFullValueName)
	{
		return EnumPtr->GetNameByValue((int64)Value).ToString();
	}

	FString Raw = EnumPtr->GetNameByValue((int64)Value).ToString();
	FString L, R;
	Raw.Split(Name + "::", &L, &R);
	return R;
}

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	return (EnumType)Enum->FindEnumIndex(FName(*String));
}

// This makes a lot of the blueprint functions cleaner
UENUM()
enum class EExtraSwitch : uint8
{
	// On Success
	OnSucceeded,
	// On Failure
	OnFailed

	// meta = (ExpandEnumAsExecs = "Result")
	// EExtraSwitch &Result
};

UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	// IOS & Android
	PT_Mobile UMETA(DisplayName = "Mobile"),
	// Xbox, Switch, PS4
	PT_Console UMETA(DisplayName = "Console"),
	// Personal Computer like Mac, Windows, Linux
	PT_PC UMETA(DisplayName = "Personal Computer")
};

UENUM(BlueprintType)
enum class EFocusCausedBy : uint8
{
	/** Focus was changed because of a mouse action. */
	Mouse					UMETA(DisplayName = "Mouse"),

	/** Focus was changed in response to a navigation, such as the arrow keys, TAB key, controller DPad, ... */
	Navigation				UMETA(DisplayName = "Navigation"),

	/** Focus was changed because someone asked the application to change it. */
	SetDirectly				UMETA(DisplayName = "Set Directly"),

	/** Focus was explicitly cleared via the escape key or other similar action. */
	Cleared					UMETA(DisplayName = "Cleared"),

	/** Focus was changed because another widget lost focus, and focus moved to a new widget. */
	OtherWidgetLostFocus	UMETA(DisplayName = "Other Widget Lost Focus"),

	/** Focus was set in response to the owning window being activated. */
	WindowActivate			UMETA(DisplayName = "Window Activate")
};

USTRUCT(BlueprintType)
struct FExtraSplineConstructionInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction")
	UStaticMesh* SplineMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction")
	TArray<UMaterialInterface*> OptionalMaterials;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction", meta = (ClampMin = 1.0f))
	float SplineTileLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction")
	FVector2D StartScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction")
	FVector2D EndScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Collision")
	uint8 bAffectNavigation : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Collision")
	uint8 bGenerateOverlapEvents : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Collision")
	TEnumAsByte<ECollisionEnabled::Type> CollisionEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Collision")
	TEnumAsByte<EObjectTypeQuery> ObjectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Collision")
	TEnumAsByte<EComponentMobility::Type> Mobility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Debug")
	uint8 bDebugMode : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Debug", meta = (ClampMin = 0.0f))
		float ArrowLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Debug", meta = (ClampMin = 0.0f))
		float ArrowSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Debug", meta = (ClampMin = 0.0f))
		float ArrowThickness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Construction|Debug", meta = (ClampMin = 0.0f))
		float DebugTime;

	FExtraSplineConstructionInfo();

};

