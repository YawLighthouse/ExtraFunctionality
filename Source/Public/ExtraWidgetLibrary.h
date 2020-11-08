#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlateCore/Public/Styling/SlateTypes.h"
#include "Types/SlateEnums.h"
#include "ExtraDataTypes.h"
#include "ExtraWidgetLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExtraWidgetLibrary, Log, All);

class UCheckBox;
class UTextBlock;
class UScrollBox;
class UUserWidget;
class UWidget;

UCLASS()
class EXTRAFUNCTIONALITY_API UExtraWidgetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "UMG")
		static bool IsTouchKey(const FKey InKey);

	UFUNCTION(BlueprintCallable, Category = "UMG")
		static void ForceUpdateMouse();

	/** Clears user focus for all users */
	UFUNCTION(BlueprintCallable, Category = "UMG")
		static void ClearAllUserFocus();

	/** Clears user focus for a certain user index */
	UFUNCTION(BlueprintCallable, Category = "UMG")
		static void ClearUserFocus(const int UserIndex);

	UFUNCTION(BlueprintPure, Category = "UMG")
		static UWidget* GetFocusedWidget();

	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|UI", meta =
		(CompactNodeTitle = "->", BlueprintAutocast))
		static EFocusCausedBy GetFocusCauseFromEvent(const FFocusEvent& InEvent);

	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|UI", meta =
		(CompactNodeTitle = "->", BlueprintAutocast))
		static FString FocusEventToString(const FFocusEvent& InEvent);

	/** Returns the UMG DPI scale found in the project settings under the UMG category */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|UI")
		static float GetDPIScale();

	/**
	* Finds all widgets of type Widget Class in Parent Widget.
	* @param ParentWidget The widget to use as a parent, will look inside this widget's tree if it contains any widgets of WidgetClass.
	* @param WidgetClass The widget class to search for inside ParentWidget's widget tree. Can return an empty array if none found.
	*/
	UFUNCTION(BlueprintCallable, Category = "UMG",
		meta = (DeterminesOutputType = "WidgetClass", DynamicOutputParam = "FoundWidgets"))
		static void GetAllWidgetsOfTypeInUserWidget(UUserWidget* ParentWidget,
			TSubclassOf<UWidget> WidgetClass, TArray<UWidget*>& FoundWidgets);

	/** Returns the user widget of this widget, can return null if not from a user widget. */
	UFUNCTION(BlueprintPure, Category = "UMG")
		static UUserWidget* GetParentUserWidget(UWidget* InWidget);

	/** Returns the root widget in this widget's hierarchy. */
	UFUNCTION(BlueprintPure, Category = "UMG")
		static UWidget* GetRootParent(UWidget* InWidget);

	/** 
	* @param bRecursive If true will recursively search through widget tree for the inputted widget name, if false will only search down immediate tree
	*/
	UFUNCTION(BlueprintPure, Category = "UMG")
	static UWidget* GetWidgetFromName(UUserWidget* InWidget, const FName InWidgetName, const bool bRecursive = false);

	UFUNCTION(BlueprintPure, Category = "UMG")
	static UWidgetAnimation* GetUserWidgetAnimation(UUserWidget* InWidget, const FName AnimName);

	static UWidgetAnimation* GetUserWidgetAnimation(UWidgetBlueprintGeneratedClass* WidgetGenClass, const FName AnimName);

#pragma region Panel Stuff

	UFUNCTION(BlueprintPure, Category = "UMG|Panel")
		static UWidget* GetTopVisibleWidget(UPanelWidget* InPanel);

	UFUNCTION(BlueprintPure, Category = "UMG|Panel")
		static UWidget* GetBottomVisibleWidget(UPanelWidget* InPanel);

	/** Returns true if Target is valid and InIndexToCheck is a valid index in Target's children */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|UI|Panel")
		static bool IsValidChildIndexInPanel(UPanelWidget* Target, int InIndexToCheck);

#pragma endregion

#pragma region Scrollbox stuff

	/** Returns the offset of the inputted widget if its a child */
	UFUNCTION(BlueprintPure, Category = "UMG|ScrollBox")
		static float GetOffsetOfScrollboxWidget(UScrollBox * InScrollbox, UWidget * ScrollBoxChild);

	/** Returns the max offset of the scrollbox */
	UFUNCTION(BlueprintPure, Category = "UMG|ScrollBox")
		static float GetScrollMaxOffset(UScrollBox* InScrollbox);

	UFUNCTION(BlueprintPure, Category = "UMG|ScrollBox")
		static float GetScrollComponentFromVector(UScrollBox* InScrollbox, FVector2D Vector);

	/** Returns true if widget is in view and false if the widget is scrolled out of view/not a child/not visible. */
	UFUNCTION(BlueprintPure, Category = "UMG|ScrollBox")
		static bool IsChildInViewOnScrollbox(UScrollBox* InScrollbox, UWidget* ScrollBoxChild);

#pragma endregion

#pragma region Checkbox stuff

	/**
	* Sets all images in the checkbox style state to InImage.
	* @param StateToSet The state to modify. EX: If StateToSet = Checked, then sets Checked Image, Checked Hovered Image, and Checked Pressed Image's to InImage.
	* @return Returns the modified style.
	*/
	UFUNCTION(BlueprintPure, Category = "UMG|Checkbox")
		static FCheckBoxStyle SetCheckboxStyleToImage(FCheckBoxStyle InStyle,
			UObject* InImage, ECheckBoxState StateToSet = ECheckBoxState::Checked);

	/** Returns the current image being used in this checkbox. */
	UFUNCTION(BlueprintPure, Category = "UMG|Checkbox")
		static UObject* GetCurrentCheckboxImage(UCheckBox* InCheckbox);

#pragma endregion

#pragma region Textblock stuff

	UFUNCTION(BlueprintCallable, Category = "UMG|Textblock")
		static void SetFontFamily(UTextBlock* Target, UObject* NewFamily);

	UFUNCTION(BlueprintCallable, Category = "UMG|Textblock")
		static void SetFontMaterial(UTextBlock* Target, UObject* NewMaterial);

	UFUNCTION(BlueprintCallable, Category = "UMG|Textblock")
		static void SetFontOutline(UTextBlock* Target, FFontOutlineSettings NewOutline);

	UFUNCTION(BlueprintCallable, Category = "UMG|Textblock")
		static void SetFontTypeface(UTextBlock* Target, FName NewTypeface);

	UFUNCTION(BlueprintCallable, Category = "UMG|Textblock")
		static void SetFontSize(UTextBlock* Target, int NewSize);

#pragma endregion

};