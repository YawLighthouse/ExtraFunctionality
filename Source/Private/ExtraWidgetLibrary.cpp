#include "ExtraWidgetLibrary.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Widget.h"
#include "Engine/Engine.h"
#include "Framework/Application/SlateApplication.h"
#include "GenericPlatform/GenericApplication.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"
#include "Runtime/Slate/Public/SlateBasics.h"
#include "UMG/Public/Components/CheckBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ScrollBox.h"

DEFINE_LOG_CATEGORY(LogExtraWidgetLibrary);

bool UExtraWidgetLibrary::IsTouchKey(const FKey InKey)
{
	if (InKey.IsValid())
	{
		return InKey.IsTouch();
	}
	return false;
}

void UExtraWidgetLibrary::ForceUpdateMouse()
{
	if (!FSlateApplication::IsInitialized())
	{
		return;
	}

	FSlateApplication::Get().QueryCursor();
}

void UExtraWidgetLibrary::ClearAllUserFocus()
{
	if (!FSlateApplication::IsInitialized())
	{
		return;
	}
	FSlateApplication::Get().ClearAllUserFocus(EFocusCause::Mouse);
	// This is fixing the focus to the game and widget's
	FSlateApplication::Get().SetAllUserFocusToGameViewport(EFocusCause::SetDirectly);
}

void UExtraWidgetLibrary::ClearUserFocus(const int UserIndex)
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().ClearUserFocus(UserIndex, EFocusCause::Cleared);
	}
}

UWidget * UExtraWidgetLibrary::GetFocusedWidget()
{
	if (FSlateApplication::IsInitialized())
	{
		for (TObjectIterator<UWidget> Itr; Itr; ++Itr)
		{
			if (Itr->HasAnyUserFocus())
			{
				return *Itr;
			}
		}
	}
	return nullptr;
}

void UExtraWidgetLibrary::GetAllWidgetsOfTypeInUserWidget(UUserWidget * ParentWidget, TSubclassOf<UWidget> WidgetClass, TArray<UWidget*>& FoundWidgets)
{
	FoundWidgets.Reset(); // Wipe the Found Widgets array
	if (!ParentWidget)
	{
		return;
	}

	// Loop through each widget in the widget tree
	ParentWidget->WidgetTree->ForEachWidget([&](UWidget* Widget)
	{		
		// If the widget's class is of type WidgetClass
		if (Widget->IsA(WidgetClass))
		{
			// Then we found one of those widget's we're looking for
			FoundWidgets.Add(Widget);
		}
	});
}

UUserWidget * UExtraWidgetLibrary::GetParentUserWidget(UWidget * InWidget)
{
	if (InWidget)
	{
		if (UWidgetTree* WidgetTree = Cast<UWidgetTree>(InWidget->GetOuter()))
		{
			if (UUserWidget* OuterWidget = Cast<UUserWidget>(WidgetTree->GetOuter()))
			{
				// Sanity check to make sure its not sending itself
				if (OuterWidget != InWidget)
				{
					return OuterWidget;
				}
			}
		}
	}
	return nullptr;
}

UWidget * UExtraWidgetLibrary::GetRootParent(UWidget * InWidget)
{
	if (InWidget)
	{
		if (UWidgetTree* WidgetTree = Cast<UWidgetTree>(InWidget->GetOuter()))
		{
			return WidgetTree->RootWidget;
		}
	}
	return nullptr;
}

UWidget* UExtraWidgetLibrary::GetWidgetFromName(UUserWidget* InWidget, const FName InWidgetName, const bool bRecursive /*= true*/)
{
	if (InWidget && !InWidgetName.IsNone())
	{
		UWidget* FoundWidget = nullptr;
		if (UWidgetTree* const Tree = InWidget->WidgetTree)
		{
			if(bRecursive)
			{
				Tree->ForEachWidgetAndDescendants([&](UWidget* SearchWidget)
					{
						if (SearchWidget->GetFName() == InWidgetName)
						{
							FoundWidget = SearchWidget;
						}
					});
			}
			else
			{
				Tree->ForEachWidget([&](UWidget* SearchWidget)
					{
						if (SearchWidget->GetFName() == InWidgetName)
						{
							FoundWidget = SearchWidget;
						}
					});
			}
		}

		return FoundWidget;
	}
	return nullptr;
}

UWidgetAnimation* UExtraWidgetLibrary::GetUserWidgetAnimation(
UUserWidget* InWidget, const FName AnimName)
{
	if (InWidget && !AnimName.IsNone())
	{
		if (UWidgetBlueprintGeneratedClass* GenClass = Cast<UWidgetBlueprintGeneratedClass>(InWidget->GetClass()))
		{
			return GetUserWidgetAnimation(GenClass, AnimName);
		}
	}
	return nullptr;
}

UWidgetAnimation* UExtraWidgetLibrary::GetUserWidgetAnimation(
UWidgetBlueprintGeneratedClass* WidgetGenClass, const FName AnimName)
{
	if (WidgetGenClass && !AnimName.IsNone())
	{
		for (UWidgetAnimation* Anim : WidgetGenClass->Animations)
		{
			if (Anim->MovieScene)
			{
				if (AnimName == Anim->MovieScene->GetFName())
				{
					return Anim;
				}
			}
		}
	}
	return nullptr;
}

UWidget * UExtraWidgetLibrary::GetTopVisibleWidget(UPanelWidget * InPanel)
{
	if (InPanel)
	{
		for (UWidget* Element : InPanel->GetAllChildren())
		{
			if (Element && Element->IsVisible())
			{
				return Element;
			}
		}
	}
	return nullptr;
}

UWidget * UExtraWidgetLibrary::GetBottomVisibleWidget(UPanelWidget * InPanel)
{
	if (InPanel)
	{
		// Do a reverse for loop to get the first visible widget going from back to front
		for (int32 index = InPanel->GetChildrenCount(); index-- > 0;)
		{
			if (UWidget* Element = InPanel->GetChildAt(index))
			{
				if (Element->IsVisible())
				{
					return Element;
				}
			}
		}
	}
	return nullptr;
}

EFocusCausedBy UExtraWidgetLibrary::GetFocusCauseFromEvent(const FFocusEvent & InEvent)
{
	EFocusCausedBy Cause = EFocusCausedBy::Cleared;

	switch (InEvent.GetCause())
	{
	case EFocusCause::Cleared:
	{
		Cause = EFocusCausedBy::Cleared;
		break;
	}
	case EFocusCause::Mouse:
	{
		Cause = EFocusCausedBy::Mouse;
	}
	case EFocusCause::Navigation:
	{
		Cause = EFocusCausedBy::Navigation;
		break;
	}
	case EFocusCause::OtherWidgetLostFocus:
	{
		Cause = EFocusCausedBy::OtherWidgetLostFocus;
		break;
	}
	case EFocusCause::SetDirectly:
	{
		Cause = EFocusCausedBy::SetDirectly;
		break;
	}
	case EFocusCause::WindowActivate:
	{
		Cause = EFocusCausedBy::WindowActivate;
		break;
	}
	default:
		break;
	}
	return Cause;
}

FString UExtraWidgetLibrary::FocusEventToString(const FFocusEvent& InEvent)
{
	return GetEnumValueAsString<EFocusCausedBy>("EFocusCausedBy", GetFocusCauseFromEvent(InEvent));
}

float UExtraWidgetLibrary::GetDPIScale()
{
	if (UGameViewportClient* Viewport = GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		Viewport->GetViewportSize(ViewportSize);		
		GetMutableDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(
				FMath::FloorToInt(ViewportSize.X), 
				FMath::FloorToInt(ViewportSize.Y)));
	}
	return 0.0f;
}

bool UExtraWidgetLibrary::IsValidChildIndexInPanel(UPanelWidget * Target, int InIndexToCheck)
{
	if (!Target)
	{
		return false;
	}

	const int ChildCount = (Target->GetChildrenCount() - 1);
	return (ChildCount >= InIndexToCheck && InIndexToCheck >= 0); // As long as child count is greater than/equal to Index AND Index is greater than/equal to zero
}

float UExtraWidgetLibrary::GetOffsetOfScrollboxWidget(UScrollBox * InScrollbox, UWidget * ScrollBoxChild)
{
	// Valid check
	if (InScrollbox && ScrollBoxChild)
	{
		// Check that its a child of this scrollbox
		if (InScrollbox->HasChild(ScrollBoxChild))
		{
			// Get the widget's position
			const float WidgetPosition = GetScrollComponentFromVector(InScrollbox,
				InScrollbox->GetCachedGeometry().AbsoluteToLocal(
					ScrollBoxChild->GetCachedGeometry().GetAbsolutePosition()) + (ScrollBoxChild->GetCachedGeometry().GetLocalSize() * 0.5f));
			// Local half size
			const float LocalHalfSize = (GetScrollComponentFromVector(InScrollbox, InScrollbox->GetCachedGeometry().GetLocalSize()) * 0.5f);
			// Returns the offset(this can return negative values)
			return (WidgetPosition - LocalHalfSize);
		}
	}
	return 0.0f;
}

float UExtraWidgetLibrary::GetScrollMaxOffset(UScrollBox * InScrollbox)
{
	if (!InScrollbox)
	{
		return 0.0f;
	}
	// Then get the size of the widget
	const float ContentSize = UExtraWidgetLibrary::GetScrollComponentFromVector(InScrollbox, InScrollbox->GetDesiredSize());
	// Then get the local size via its slot
	const float LocalSize = UExtraWidgetLibrary::GetScrollComponentFromVector(InScrollbox, InScrollbox->GetCachedGeometry().GetLocalSize());
	// The difference between the two is the maximum amount
	return (ContentSize - LocalSize);
}

float UExtraWidgetLibrary::GetScrollComponentFromVector(UScrollBox * InScrollbox, FVector2D Vector)
{
	if (InScrollbox)
	{
		return InScrollbox->Orientation == Orient_Vertical ? Vector.Y : Vector.X;
	}
	return 0.0f;
}

bool UExtraWidgetLibrary::IsChildInViewOnScrollbox(UScrollBox * InScrollbox, UWidget * ScrollBoxChild)
{
	// Valid check
	if (InScrollbox && ScrollBoxChild)
	{
		// Check that its a child of this scrollbox
		if (InScrollbox->HasChild(ScrollBoxChild))
		{
			// Gets how far the widget has scrolled from center
			const float Offset = UExtraWidgetLibrary::GetOffsetOfScrollboxWidget(InScrollbox, ScrollBoxChild);
			// Local half size of the scrollbox
			const float LocalHalfSize = (UExtraWidgetLibrary::GetScrollComponentFromVector(InScrollbox, InScrollbox->GetCachedGeometry().GetLocalSize()) * 0.5f);
			// Check if the offset under the bounds max
			return (FMath::Abs(Offset) < LocalHalfSize);
		}
	}
	return false;
}

void UExtraWidgetLibrary::SetFontFamily(UTextBlock * Target, UObject * NewFamily)
{
	if (Target)
	{
		FSlateFontInfo TempFont = Target->Font;
		TempFont.FontObject = NewFamily;
		Target->SetFont(TempFont);
	}
}

void UExtraWidgetLibrary::SetFontMaterial(UTextBlock * Target, UObject * NewMaterial)
{
	if (Target)
	{
		FSlateFontInfo TempFont = Target->Font;
		TempFont.FontMaterial = NewMaterial;
		Target->SetFont(TempFont);
	}
}

void UExtraWidgetLibrary::SetFontOutline(UTextBlock * Target, FFontOutlineSettings NewOutline)
{
	if (Target)
	{
		FSlateFontInfo TempFont = Target->Font;
		TempFont.OutlineSettings = NewOutline;
		Target->SetFont(TempFont);
	}
}

void UExtraWidgetLibrary::SetFontTypeface(UTextBlock * Target, FName NewTypeface)
{
	if (Target && !NewTypeface.IsNone())
	{
		FSlateFontInfo TempFont = Target->Font;
		TempFont.TypefaceFontName = NewTypeface;
		Target->SetFont(TempFont);
	}
}

void UExtraWidgetLibrary::SetFontSize(UTextBlock * Target, int NewSize)
{
	if (Target)
	{
		FSlateFontInfo TempFont = Target->Font;
		// Keeps it within the allowed bounds
		TempFont.Size = FMath::Clamp(NewSize, 1, 1000);
		Target->SetFont(TempFont);
	}
}

FCheckBoxStyle UExtraWidgetLibrary::SetCheckboxStyleToImage(FCheckBoxStyle InStyle, UObject * InImage, ECheckBoxState StateToSet)
{
	if (!InImage)
	{
		return InStyle;
	}

	FSlateBrush Brush;
	Brush.SetResourceObject(InImage);

	switch (StateToSet)
	{
	case ECheckBoxState::Unchecked:
	{
		InStyle.SetUncheckedHoveredImage(Brush);
		InStyle.SetUncheckedImage(Brush);
		InStyle.SetUncheckedPressedImage(Brush);
		break;
	}
	case ECheckBoxState::Checked:
	{
		InStyle.SetCheckedHoveredImage(Brush);
		InStyle.SetCheckedImage(Brush);
		InStyle.SetCheckedPressedImage(Brush);
		break;
	}
	case ECheckBoxState::Undetermined:
	default:
	{
		InStyle.SetUndeterminedHoveredImage(Brush);
		InStyle.SetUndeterminedImage(Brush);
		InStyle.SetUndeterminedPressedImage(Brush);
		break;
	}
	}

	return InStyle;
}

UObject * UExtraWidgetLibrary::GetCurrentCheckboxImage(UCheckBox * InCheckbox)
{
	if (!InCheckbox)
	{
		return nullptr;
	}

	UObject* FoundImage = nullptr;
	switch (InCheckbox->GetCheckedState())
	{
	case ECheckBoxState::Unchecked:
	{
		if (InCheckbox->IsHovered())
		{
			FoundImage = InCheckbox->WidgetStyle.UncheckedHoveredImage.GetResourceObject();
		}
		else if (InCheckbox->IsPressed())
		{
			FoundImage = InCheckbox->WidgetStyle.UncheckedPressedImage.GetResourceObject();
		}
		else
		{
			FoundImage = InCheckbox->WidgetStyle.UncheckedImage.GetResourceObject();
		}
		break;
	}
	case ECheckBoxState::Checked:
	{
		if (InCheckbox->IsHovered())
		{
			FoundImage = InCheckbox->WidgetStyle.CheckedHoveredImage.GetResourceObject();
		}
		else if (InCheckbox->IsPressed())
		{
			FoundImage = InCheckbox->WidgetStyle.CheckedPressedImage.GetResourceObject();
		}
		else
		{
			FoundImage = InCheckbox->WidgetStyle.CheckedImage.GetResourceObject();
		}
		break;
	}
	case ECheckBoxState::Undetermined:
	default:
	{
		if (InCheckbox->IsHovered())
		{
			FoundImage = InCheckbox->WidgetStyle.UndeterminedHoveredImage.GetResourceObject();
		}
		else if (InCheckbox->IsPressed())
		{
			FoundImage = InCheckbox->WidgetStyle.UndeterminedPressedImage.GetResourceObject();
		}
		else
		{
			FoundImage = InCheckbox->WidgetStyle.UndeterminedImage.GetResourceObject();
		}
		break;
	}
	}

	return FoundImage;
}
