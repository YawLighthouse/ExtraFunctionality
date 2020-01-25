#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "Engine/EngineTypes.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "ExtraDataTypes.generated.h"

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
	/** Xbox One */
	XboxOne UMETA(DisplayName = "Xbox One"),
	/** Playstation 4*/
	PS4 UMETA(DisplayName = "PS4"),
	/** Nintendo Switch */
	Switch UMETA(DisplayName = "Switch"),
	/** Windows, Mac, or Linux */
	Desktop UMETA(DisplayName = "Desktop")
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
