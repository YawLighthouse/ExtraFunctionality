

#include "ExtraFunctionalityLibrary.h"
#include "Components/SceneComponent.h"
#include "ConfigCacheIni.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Engine/Console.h"
#include "Engine/Classes/GameFramework/GameMode.h"
#include "GenericPlatformMisc.h"
#include "ExtraMathLibrary.h"
#include "HAL/FileManager.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkReplayStreaming/Public/NetworkReplayStreaming.h"
#include "Paths.h"
#include "Runtime/Engine/Classes/Engine/DemoNetDriver.h"
#include "UMG/Public/Components/CheckBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UserWidget.h"
#include "WidgetTree.h"

DEFINE_LOG_CATEGORY(LogExtraFunctionalityLibrary);

DECLARE_LOG_CATEGORY_EXTERN(LogCustomBlueprintCategory, Log, All);
DEFINE_LOG_CATEGORY(LogCustomBlueprintCategory);


void UExtraFunctionalityLibrary::ForceCrash(const FString& CrashMessage)
{
#if !UE_BUILD_SHIPPING
	UE_LOG(LogExtraFunctionalityLibrary, Fatal, TEXT("FORCING CRASH TO HAPPEN: %s"), *CrashMessage);

	UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("[%s]"), *"CRASH CRASH BAYBEEE");
#endif
}

void UExtraFunctionalityLibrary::RequestExit(bool bForce)
{
	FGenericPlatformMisc::RequestExit(bForce);
}

void UExtraFunctionalityLibrary::GetAllLevels(UObject* WorldContextObject, TArray<ULevel*>& Levels)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	Levels = World->GetLevels();
}

TSubclassOf<class UObject> UExtraFunctionalityLibrary::GetClassFromAssetPath(FString Path)
{
		TSubclassOf<class UObject> AssetToCheck = NULL;

		UObject* Asset = StaticLoadObject(UObject::StaticClass(), nullptr, *Path);
		if (Asset != NULL)
		{
			UBlueprint* bp = Cast<UBlueprint>(Asset);
			if (bp != NULL)
			{
				//if (GEngine)
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Found the Asset")));
				AssetToCheck = (UClass*)bp->GeneratedClass;
			}
			else
			{
				//if (GEngine)
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Blueprint Asset is NULL")));
			}
		}
		else
		{
			//if (GEngine)
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Asset is NULL")));
		}

		return AssetToCheck;
}

FString UExtraFunctionalityLibrary::GetLocalAppDataDirectory()
{
	return (FPaths::ConvertRelativePathToFull(FPaths::ProjectUserDir()));
}

bool UExtraFunctionalityLibrary::DeleteDirectory(FString InDir)
{
	if (FPaths::DirectoryExists(InDir))
	{
		return IFileManager::Get().DeleteDirectory(*InDir, false, true);
	}
	UE_LOG(LogExtraFunctionalityLibrary, Warning, TEXT("Directory: [%s] does not exist, so we're counting this as already deleted!"), *InDir);
	return true;
}

bool UExtraFunctionalityLibrary::DeleteFile(FString InFileDir)
{
	if (FPaths::FileExists(InFileDir))
	{
		return IFileManager::Get().Delete(*InFileDir, false, true, true);
	}
	UE_LOG(LogExtraFunctionalityLibrary, Warning, TEXT("File: [%s] does not exist, so we're counting this as already deleted!"), *InFileDir);
	return true;
}

FString UExtraFunctionalityLibrary::IncreaseVerbosityOfMessage(FString InMessage, bool bIsError)
{	
	return (((bIsError) ? "error:" : "warning:") + InMessage);
}

void UExtraFunctionalityLibrary::PrintStringWithCategory(UObject * WorldContextObject, 
	const FString & InString, FString InCategory, 
	bool bPrintToScreen, bool bPrintToLog, FLinearColor TextColor, float Duration)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	FString Prefix;
	if (World)
	{
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server: "));
				break;
			case NM_Standalone:
				break;
			}
		}
	}

	if (InCategory.IsEmpty())
	{
		InCategory = "EmptyCustomCategory";
	}

	const FString FinalDisplayString = Prefix + InString;
	FString FinalLogString = FinalDisplayString;

	static const FBoolConfigValueHelper DisplayPrintStringSource(TEXT("Kismet"), TEXT("bLogPrintStringSource"), GEngineIni);
	if (DisplayPrintStringSource)
	{
		const FString SourceObjectPrefix = FString::Printf(TEXT("[%s] "), *GetNameSafe(WorldContextObject));
		FinalLogString = SourceObjectPrefix + FinalLogString;
	}

	if (bPrintToLog)
	{
		UE_LOG(LogCustomBlueprintCategory, Log, TEXT("%s:: %s"), *InCategory, *FinalLogString);

		APlayerController* PC = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : NULL);
		ULocalPlayer* LocalPlayer = (PC ? Cast<ULocalPlayer>(PC->Player) : NULL);
		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->ViewportConsole)
		{
			LocalPlayer->ViewportClient->ViewportConsole->OutputText(FinalDisplayString);
		}
	}
	else
	{
		UE_LOG(LogCustomBlueprintCategory, Verbose, TEXT("%s:: %s"), *InCategory, *FinalLogString);
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) // Do not Print in Shipping or Test

	// Also output to the screen, if possible
	if (bPrintToScreen)
	{
		if (GAreScreenMessagesEnabled)
		{
			if (GConfig && Duration < 0)
			{
				GConfig->GetFloat(TEXT("Kismet"), TEXT("PrintStringDuration"), Duration, GEngineIni);
			}
			GEngine->AddOnScreenDebugMessage((uint64)-1, Duration, TextColor.ToFColor(true), FinalDisplayString);
		}
		else
		{
			UE_LOG(LogCustomBlueprintCategory, VeryVerbose, TEXT("Screen messages disabled (!GAreScreenMessagesEnabled).  Cannot print to screen."));
		}
	}
#endif
}

FString UExtraFunctionalityLibrary::ReplaceUnderscore(FString SourceString)
{
	return SourceString.Replace(TEXT("_"), TEXT(" "));
}

FString UExtraFunctionalityLibrary::RemoveSpaces(FString SourceString)
{
	return SourceString.Replace(TEXT(" "), TEXT(""));
}

TArray<FString> UExtraFunctionalityLibrary::GetConnectedClientIPs(const UObject * WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (World->GetNetDriver() && World->GetNetMode() != NM_Client)
		{
			TArray<FString> IPAddresses;
			for (UNetConnection* Client : World->GetNetDriver()->ClientConnections)
			{
				// Clients can be null because they disconnected so we're only showing the currently connected clients
				if (Client)
				{
					IPAddresses.Add(Client->RemoteAddressToString());
				}
			}
			return IPAddresses;
		}
	}
	return TArray<FString>();
}

FString UExtraFunctionalityLibrary::GetServerIpFromGamemode(AGameMode * InGamemode)
{
	if (InGamemode)
	{
		return InGamemode->GetNetworkNumber();
	}
	return FString();
}

FString UExtraFunctionalityLibrary::GetServerIP(const UObject * WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (World->GetNetDriver() && World->GetNetMode() == NM_Client)
		{
			// Incase we're calling this on a client this will be null and fail cleanly
			if (World->GetNetDriver()->ServerConnection)
			{
				return World->GetNetDriver()->ServerConnection->RemoteAddressToString();
			}
		}
	}
	return FString();
}

FString UExtraFunctionalityLibrary::GetPlayerIP(const APlayerController* InPlayer)
{
	if (InPlayer)
	{
		if (InPlayer->GetNetConnection())
		{
			return InPlayer->GetNetConnection()->RemoteAddressToString();
		}
	}
	return FString();
}

FVector UExtraFunctionalityLibrary::GetSocketRelativeLocation(USceneComponent * Target, FName InSocketName)
{
	if (Target)
	{
		if (AActor* TargetOwner = Target->GetOwner())
		{
			return TargetOwner->GetActorTransform().InverseTransformPosition(Target->GetSocketLocation(InSocketName));
		}
	}
	return FVector();
}

FRotator UExtraFunctionalityLibrary::GetSocketRelativeRotation(USceneComponent * Target, FName InSocketName)
{
	if (Target)
	{
		if (AActor* TargetOwner = Target->GetOwner())
		{
			return TargetOwner->GetActorTransform().InverseTransformRotation(Target->GetSocketQuaternion(InSocketName)).Rotator();
		}
	}
	return FRotator();
}

bool UExtraFunctionalityLibrary::MarkRenderDirty(USceneComponent * InComp)
{
	if (!InComp)
	{
		return false;
	}

	InComp->MarkRenderDynamicDataDirty();
	InComp->MarkRenderTransformDirty();
	InComp->MarkRenderStateDirty();
	return true;
}

bool UExtraFunctionalityLibrary::AreObjectsValid(TArray<UObject*> Objects)
{
	if (Objects.Num() == 0)
	{
		return false;
	}

	for (int32 index = Objects.Num(); index-- > 0;)
	{
		// If we run into an invalid object, return that we failed.
		if (!IsValid(Objects[index]))
		{
			return false;
		}
	}

	// If we didnt return then all of them are valid! :D
	return true;
}

bool UExtraFunctionalityLibrary::IsOverlappingAnyActors(UPrimitiveComponent * InComp, bool bExcludeSelf)
{
	// Valid check
	if (InComp)
	{		
		TArray<AActor*> FoundActors;
		InComp->GetOverlappingActors(FoundActors);

		// Remove the component's actor
		if (bExcludeSelf)
		{
			FoundActors.Remove(InComp->GetOwner());
		}

		// Check that the array isn't empty
		return (FoundActors.Num() > 0);
	}
	return false;
}

bool UExtraFunctionalityLibrary::FindFirstInstanceOfActorType(const UObject * WorldContextObject, TSubclassOf<AActor> SearchClass, AActor *& FoundActor)
{
	FoundActor = nullptr;

	// Valid checks
	if (!WorldContextObject || !SearchClass)
	{
		return false;
	}
	// Get the world to iterate through
	else if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// Loop through each actor in the world that is the same class as search class
		for (TActorIterator<AActor> ActorItr(World, SearchClass); ActorItr; ++ActorItr)
		{			
			AActor* Actor = *ActorItr;
			if (!Actor->IsPendingKill())
			{
				// When we started the actor iterator we told it to filter only for the search class so all of these actors are of the class we're looking for
				FoundActor = Actor; // So lets return that first one
				break;
			}
		}
	}

	return (FoundActor != nullptr);
}

void UExtraFunctionalityLibrary::MarkRenderDity_Comps(TArray<USceneComponent*> InComps)
{
	for (USceneComponent* Comp : InComps)
	{
		MarkRenderDirty(Comp);
	}
}

FVector2D UExtraFunctionalityLibrary::GetDirectionalInputsFromDirectionalKeys(const APlayerController * InPlayerController,
	const FKey UpDirectional, const FKey DownDirectional, const FKey RightDirectional, const FKey LeftDirectional)
{
	if (!InPlayerController)
	{
		return FVector2D();
	}

	const FVector2D FinalInput = FVector2D(
		((float)InPlayerController->IsInputKeyDown(LeftDirectional) * -1.0) + (float)InPlayerController->IsInputKeyDown(RightDirectional), // X
		((float)InPlayerController->IsInputKeyDown(DownDirectional) * -1.0f) + (float)InPlayerController->IsInputKeyDown(UpDirectional) // Y
	);

	return FinalInput;
}

float UExtraFunctionalityLibrary::GetAxisValueOfInputs(const APlayerController * InPlayerController, const FKey PositiveKey, const FKey NegativeKey)
{
	if (!InPlayerController)
	{
		return 0.0f;
	}

	return ((float)InPlayerController->IsInputKeyDown(NegativeKey) * -1.0) + (float)InPlayerController->IsInputKeyDown(PositiveKey);
}

int UExtraFunctionalityLibrary::GetLastMaterialIndex(UPrimitiveComponent * Target)
{
	if (!Target)
	{
		return -1;
	}

	return (Target->GetNumMaterials() - 1);
}

void UExtraFunctionalityLibrary::ClearInputMappings(UInputSettings * const InSettings, bool bForceRebuildKeymaps, bool bSaveKeyMappings)
{
	// Dont continue if invalid
	if (!InSettings)
	{
		return;
	}

	InSettings->AxisMappings.Empty();
	InSettings->ActionMappings.Empty();

	if (bForceRebuildKeymaps)
	{
		InSettings->ForceRebuildKeymaps();
	}

	if (bSaveKeyMappings)
	{
		InSettings->SaveKeyMappings();
	}
}

bool UExtraFunctionalityLibrary::Equals_InputAxisKeyMapping(FInputAxisKeyMapping A, FInputAxisKeyMapping B, bool bNameOnly)
{
	if (bNameOnly)
	{
		return (A.AxisName == B.AxisName);
	}
	return (A == B);
}

bool UExtraFunctionalityLibrary::Equals_InputActionKeyMapping(FInputActionKeyMapping A, FInputActionKeyMapping B, bool bNameOnly)
{
	if (bNameOnly)
	{
		return (A.ActionName == B.ActionName);
	}
	return (A == B);
}

void UExtraFunctionalityLibrary::GetAllWidgetsOfTypeInUserWidget(UUserWidget * ParentWidget, TSubclassOf<UWidget> WidgetClass, TArray<UWidget*>& FoundWidgets)
{
	FoundWidgets.Reset(); // Wipe the Found Widgets array
	if (!ParentWidget)
	{
		return;
	}

	// Loop through each widget in the widget tree using a Lambda
	ParentWidget->WidgetTree->ForEachWidget([&](UWidget* Widget)
	{
		check(Widget); // If this widget is valid(which it should be) then we can continue

		// If the widget's class is of type WidgetClass
		if (Widget->IsA(WidgetClass))
		{
			// Then we found one of those widget's we're looking for
			FoundWidgets.Add(Widget);
		}
	});
}

void UExtraFunctionalityLibrary::SetFontFamily(UTextBlock * Target, UObject * NewFamily)
{
	if (Target)
	{
		FSlateFontInfo TempFont = Target->Font;		
		TempFont.FontObject = NewFamily;
		Target->SetFont(TempFont);
	}
}

void UExtraFunctionalityLibrary::SetFontMaterial(UTextBlock * Target, UObject * NewMaterial)
{
	if (Target)
	{
		FSlateFontInfo TempFont = Target->Font;
		TempFont.FontMaterial = NewMaterial;
		Target->SetFont(TempFont);
	}
}

void UExtraFunctionalityLibrary::SetFontOutline(UTextBlock * Target, FFontOutlineSettings NewOutline)
{
	if (Target)
	{
		FSlateFontInfo TempFont = Target->Font;
		TempFont.OutlineSettings = NewOutline;
		Target->SetFont(TempFont);
	}
}

void UExtraFunctionalityLibrary::SetFontTypeface(UTextBlock * Target, FName NewTypeface)
{
	if (Target && !NewTypeface.IsNone())
	{		
		FSlateFontInfo TempFont = Target->Font;
		TempFont.TypefaceFontName = NewTypeface;
		Target->SetFont(TempFont);
	}
}

void UExtraFunctionalityLibrary::SetFontSize(UTextBlock * Target, int NewSize)
{
	if (Target)
	{
		FSlateFontInfo TempFont = Target->Font;
		// Keeps it within the allowed bounds
		TempFont.Size = FMath::Clamp(NewSize, 1, 1000);
		Target->SetFont(TempFont);
	}
}

int UExtraFunctionalityLibrary::GetLastChildIndex(UPanelWidget * Target)
{
	if (!Target)
	{
		return -1;
	}

	return Target->GetChildrenCount() - 1;
}

bool UExtraFunctionalityLibrary::IsValidChildIndexInPanel(UPanelWidget * Target, int InIndexToCheck)
{
	if (!Target)
	{
		return false;
	}

	const int ChildCount = (Target->GetChildrenCount() - 1);
	return (ChildCount >= InIndexToCheck && InIndexToCheck >= 0); // As long as child count is greater than/equal to Index AND Index is greater than/equal to zero
}

FCheckBoxStyle UExtraFunctionalityLibrary::SetCheckboxStyleToImage(FCheckBoxStyle InStyle, UObject * InImage, ECheckBoxState StateToSet)
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

UObject * UExtraFunctionalityLibrary::GetCurrentCheckboxImage(UCheckBox * InCheckbox)
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

void UExtraFunctionalityLibrary::DrawDebugCoordinateArrowsAtComponent(USceneComponent * InComponent, FVector Offset, float ArrowLength, float ArrowSize, float ArrowThickness,
	float DebugDisplayTime, FLinearColor ForwardColor, FLinearColor RightColor, FLinearColor UpColor)
{
#if ENABLE_DRAW_DEBUG
	if (!InComponent)
	{
		return;
	}
	else if (UWorld* World = GEngine->GetWorldFromContextObject(InComponent, EGetWorldErrorMode::LogAndReturnNull))
	{
		const FVector StartLoc = (InComponent->GetComponentLocation() + Offset);

		// X
		DrawDebugDirectionalArrow(World, StartLoc, StartLoc + (InComponent->GetForwardVector() * ArrowLength), 
			ArrowSize, ForwardColor.ToFColor(true), false, DebugDisplayTime, SDPG_World, ArrowThickness);
		// Y
		DrawDebugDirectionalArrow(World, StartLoc, StartLoc + (InComponent->GetRightVector() * ArrowLength), 
		ArrowSize, RightColor.ToFColor(true), false, DebugDisplayTime, SDPG_World, ArrowThickness);
		// Z
		DrawDebugDirectionalArrow(World, StartLoc, StartLoc + (InComponent->GetUpVector() * ArrowLength), 
		ArrowSize, UpColor.ToFColor(true), false, DebugDisplayTime, SDPG_World, ArrowThickness);
	}
#endif
}

bool UExtraFunctionalityLibrary::SnapActorToGround(AActor * InActor, float TraceDistance, bool bTraceComplex, ETraceTypeQuery GroundChannel,
	const TArray<AActor*>& ActorsToIgnore, FVector OptionalOffset, bool bDrawDebug, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawDebugTime)
{
	if (InActor)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(InActor, EGetWorldErrorMode::LogAndReturnNull))
		{
			// Setup the variables to use for the trace
			ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(GroundChannel);
			FCollisionQueryParams Params;
			Params.AddIgnoredActors(ActorsToIgnore);
			Params.AddIgnoredActor(InActor); // Ignore the inputted actor
			Params.bTraceComplex = bTraceComplex;

			FVector StartLoc = InActor->GetActorLocation();
			FVector EndLoc = (StartLoc + (FVector(0.0f, 0.0f, -1.0f) * TraceDistance));
			FHitResult Hit;

			// Trace
			if (World->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, CollisionChannel, Params))
			{
				// If it hit, update the location of that actor
				InActor->SetActorLocation(Hit.ImpactPoint + OptionalOffset);

#if ENABLE_DRAW_DEBUG
				if (bDrawDebug)
				{
					DrawDebugLine(World, StartLoc, Hit.ImpactPoint, TraceColor.ToFColor(true), false, DrawDebugTime); // Start to ImpactPoint
					DrawDebugLine(World, Hit.ImpactPoint, EndLoc, TraceHitColor.ToFColor(true), false, DrawDebugTime); // ImpactPoint to End
					DrawDebugPoint(World, Hit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), false, DrawDebugTime); // ImpactPoint
				}
#endif
				return true;
			}
			else
			{
#if ENABLE_DRAW_DEBUG
				if (bDrawDebug)
				{
					DrawDebugLine(World, StartLoc, EndLoc, TraceColor.ToFColor(true), false, DrawDebugTime); // Start to End			
				}
#endif			
			}
		}
	}
	return false;
}

bool UExtraFunctionalityLibrary::IsValidSplinePoint(USplineComponent* SplineComp, int32 InPoint)
{
	if (SplineComp)
	{
		return UKismetMathLibrary::InRange_IntInt(InPoint, 0, (SplineComp->GetNumberOfSplinePoints() - 1));
	}
	return false;
}

int32 UExtraFunctionalityLibrary::GetLastSplinePoint(USplineComponent * SplineComp)
{
	if (SplineComp)
	{
		return (SplineComp->GetNumberOfSplinePoints() - 1);
	}	
	return int32();
}

void UExtraFunctionalityLibrary::SnapAllSplinePointsToGround(USplineComponent* SplineComp, float TraceDistance, bool bTraceComplex,
	ETraceTypeQuery TraceChannel, const TArray<AActor*>& ActorsToIgnore, bool bDrawDebug,
	FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawDebugTime)
{
	if (!SplineComp)
	{
		return;
	}
	else if (UWorld* World = GEngine->GetWorldFromContextObject(SplineComp, EGetWorldErrorMode::LogAndReturnNull))
	{		
		// Setup the variables to use in the for loop so we're not creating some of these variables for each loop iteration
		ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);
		FCollisionQueryParams Params;		
		Params.AddIgnoredActors(ActorsToIgnore);
		Params.AddIgnoredActor(SplineComp->GetOwner()); // Ignore spline components actor
		Params.bTraceComplex = bTraceComplex;

		FVector StartLoc = SplineComp->GetComponentLocation();
		FVector EndLoc = (StartLoc + (FVector(0.0f, 0.0f, -1.0f) * TraceDistance));
		FHitResult Hit;

		// Reverse for loop, because its faster than forward for loops
		for (int32 index = (SplineComp->GetNumberOfSplinePoints()); index-- > 0;)
		{
			// Update the trace locations
			StartLoc = SplineComp->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::World);
			EndLoc = (StartLoc + (FVector(0.0f, 0.0f, -1.0f) * TraceDistance));

			// Trace
			if (World->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, CollisionChannel, Params))
			{
				// If it hit, update the location of that spline point
				SplineComp->SetLocationAtSplinePoint(index, Hit.ImpactPoint, ESplineCoordinateSpace::World);

#if ENABLE_DRAW_DEBUG
				if (bDrawDebug)
				{
					DrawDebugLine(World, StartLoc, Hit.ImpactPoint, TraceColor.ToFColor(true), false, DrawDebugTime); // Start to ImpactPoint
					DrawDebugLine(World, Hit.ImpactPoint, EndLoc, TraceHitColor.ToFColor(true), false, DrawDebugTime); // ImpactPoint to End
					DrawDebugPoint(World, Hit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), false, DrawDebugTime); // ImpactPoint
				}
#endif
			}
			else
			{
#if ENABLE_DRAW_DEBUG
				if (bDrawDebug)
				{
					DrawDebugLine(World, StartLoc, EndLoc, TraceColor.ToFColor(true), false, DrawDebugTime); // Start to End			
				}
#endif
			}
		}
	}
}

void UExtraFunctionalityLibrary::SnapSignleSplinePointToGround(USplineComponent * SplineComp,
	int32 SplinePointToSnap, float TraceDistance, bool bTraceComplex, ETraceTypeQuery TraceChannel, 
	const TArray<AActor*>& ActorsToIgnore, bool bDrawDebug, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawDebugTime)
{
	if (!SplineComp)
	{
		return;
	}
	else if (IsValidSplinePoint(SplineComp, SplinePointToSnap))
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(SplineComp, EGetWorldErrorMode::LogAndReturnNull))
		{
			// Setup the variables to use in the for loop so we're not creating some of these variables for each loop iteration
			ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);
			FCollisionQueryParams Params;
			Params.AddIgnoredActors(ActorsToIgnore);
			Params.AddIgnoredActor(SplineComp->GetOwner()); // Ignore spline components actor
			Params.bTraceComplex = bTraceComplex;

			FVector StartLoc = SplineComp->GetLocationAtSplinePoint(SplinePointToSnap, ESplineCoordinateSpace::World);
			FVector EndLoc = (StartLoc + (FVector(0.0f, 0.0f, -1.0f) * TraceDistance));
			FHitResult Hit;
			// Trace
			if (World->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, CollisionChannel, Params))
			{
				// If it hit, update the location of that spline point
				SplineComp->SetLocationAtSplinePoint(SplinePointToSnap, Hit.ImpactPoint, ESplineCoordinateSpace::World);

#if ENABLE_DRAW_DEBUG
				if (bDrawDebug)
				{
					DrawDebugLine(World, StartLoc, Hit.ImpactPoint, TraceColor.ToFColor(true), false, DrawDebugTime); // Start to ImpactPoint
					DrawDebugLine(World, Hit.ImpactPoint, EndLoc, TraceHitColor.ToFColor(true), false, DrawDebugTime); // ImpactPoint to End
					DrawDebugPoint(World, Hit.ImpactPoint, 16.0f, TraceColor.ToFColor(true), false, DrawDebugTime); // ImpactPoint
				}
#endif
			}
			else
			{
#if ENABLE_DRAW_DEBUG
				if (bDrawDebug)
				{
					DrawDebugLine(World, StartLoc, EndLoc, TraceColor.ToFColor(true), false, DrawDebugTime); // Start to End			
				}
#endif
			}
		}
	}
}

FVector UExtraFunctionalityLibrary::FindLocationAtSplineInputKey(USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if(SplineComp)
	{
		return SplineComp->GetLocationAtSplineInputKey(InKey, CoordinateSpace);
	}
	return FVector();
}

FVector UExtraFunctionalityLibrary::FindTangentAtSplineInputKey(USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (SplineComp)
	{
		return SplineComp->GetTangentAtSplineInputKey(InKey, CoordinateSpace);
	}
	return FVector();
}

FVector UExtraFunctionalityLibrary::FindDirectionAtSplineInputKey(USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (SplineComp)
	{
		return SplineComp->GetDirectionAtSplineInputKey(InKey, CoordinateSpace);
	}
	return FVector();
}

FRotator UExtraFunctionalityLibrary::FindRotationAtSplineInputKey(USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (SplineComp)
	{
		return SplineComp->GetRotationAtSplineInputKey(InKey, CoordinateSpace);
	}
	return FRotator();
}

FVector UExtraFunctionalityLibrary::FindUpVectorAtSplineInputKey(USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (SplineComp)
	{
		return SplineComp->GetUpVectorAtSplineInputKey(InKey, CoordinateSpace);
	}
	return FVector();
}

FVector UExtraFunctionalityLibrary::FindRightVectorAtSplineInputKey(USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (SplineComp)
	{
		return SplineComp->GetRightVectorAtSplineInputKey(InKey, CoordinateSpace);
	}
	return FVector();
}

FTransform UExtraFunctionalityLibrary::FindTransformAtSplineInputKey(USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace, bool bUseScale)
{
	if (SplineComp)
	{
		return SplineComp->GetTransformAtSplineInputKey(InKey, CoordinateSpace);
	}
	return FTransform();
}

float UExtraFunctionalityLibrary::FindRollAtSplineInputKey(USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (SplineComp)
	{
		return SplineComp->GetRollAtSplineInputKey(InKey, CoordinateSpace);
	}
	return 0.0f;
}

FVector UExtraFunctionalityLibrary::FindScaleAtSplineInputKey(USplineComponent * SplineComp, float InKey)
{
	if (SplineComp)
	{
		return SplineComp->GetScaleAtSplineInputKey(InKey);
	}
	return FVector();
}

void UExtraFunctionalityLibrary::FindLocationAndRotationAtSplineInputKey(FVector & Location, FRotator & Rotation, USplineComponent * SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (SplineComp)
	{
		Location = SplineComp->GetLocationAtSplineInputKey(InKey, CoordinateSpace);
		Rotation = SplineComp->GetRotationAtSplineInputKey(InKey, CoordinateSpace);
	}	
}

void UExtraFunctionalityLibrary::StartRecordingReplay(const UObject* WorldContextObject, const FString & ReplayName,
	const FString & FriendlyName)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{						
		if(UGameInstance* const Gi = World->GetGameInstance())
		{
			UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Started recording replay, ReplayName : [%s], FriendlyName : [%s]"), *ReplayName, *FriendlyName);
			Gi->StartRecordingReplay(ReplayName, FriendlyName);
		}
		else
		{
			UE_LOG(LogExtraFunctionalityLibrary, Warning, TEXT("Failed to get game instance to start recording replay"));
		}
	}
}

void UExtraFunctionalityLibrary::StopRecordingReplay(const UObject* WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{		
		if (UGameInstance* const Gi = World->GetGameInstance())
		{
			UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Generically stopping recording of replay"));
			Gi->StopRecordingReplay();
		}
		else
		{
			UE_LOG(LogExtraFunctionalityLibrary, Warning, TEXT("Failed to get game instance to stop recording replay"));
		}
	}
}

bool UExtraFunctionalityLibrary::IsReplayCurrentlyActive(const UObject * WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->DemoNetDriver;
	}
	return false;
}

void UExtraFunctionalityLibrary::SaveReplayCheckpoint(const UObject * WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (World->DemoNetDriver)
		{
			if (!World->DemoNetDriver->IsSavingCheckpoint())
			{
				UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Started saving checkpoint at recording time: [%f]"), World->DemoNetDriver->GetDemoCurrentTime());
				World->DemoNetDriver->SaveCheckpoint();
			}
			else
			{
				UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Failed to save checkpoint: currently saving checkpoint already"));
			}
		}
		else
		{
			UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Failed to save checkpoint: Replay is not currently active"));
		}
	}
	return;
}

bool UExtraFunctionalityLibrary::PlayReplay(const UObject* WorldContextObject, const FString & ReplayName)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UGameInstance* const Gi = World->GetGameInstance())
		{
			UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Playing replay : [%s]"), *ReplayName);
			return Gi->PlayReplay(ReplayName);
		}
	}
	return false;
}

void UExtraFunctionalityLibrary::AddUserToReplay(const UObject* WorldContextObject, const FString & UserString)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UGameInstance* const Gi = World->GetGameInstance())
		{
			UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Added [%s] to replay"), *UserString);
			Gi->AddUserToReplay(UserString);
		}
	}
}

void UExtraFunctionalityLibrary::JumpToTimeInReplay(float ReplayTime, const UObject * WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		
	}
}

bool UExtraFunctionalityLibrary::IsReplayPaused(const UObject * WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{		
		if (AWorldSettings* const Settings = World->GetWorldSettings())
		{
			return (Settings->Pauser);
		}
	}
	return false;
}

void UExtraFunctionalityLibrary::ToggleReplayPauseState(const UObject * WorldContextObject)
{
	SetReplayPausedState(!IsReplayPaused(WorldContextObject), WorldContextObject);
}

static int32 PreviousAASetting = 0;
static int32 PreviousMBSetting = 0;

void UExtraFunctionalityLibrary::SetReplayPausedState(bool NewState, const UObject * WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{					
		AWorldSettings* const Settings = World->GetWorldSettings();

		const bool IsPaused = Settings->Pauser;
		if (IsPaused != NewState)
		{				
			UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Changing replay pause state to: [%s]"), (NewState) ? TEXT("PAUSED") : TEXT("UN-PAUSED"));

			// Set MotionBlur off and Anti Aliasing to FXAA in order to bypass the pause-bug of both
			static IConsoleVariable* const CVarAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.AntiAliasing"));
			static IConsoleVariable* const CVarMB = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.MotionBlur"));

			// If we are pausing
			if (NewState)
			{
				PreviousAASetting = CVarAA->GetInt();
				PreviousMBSetting = CVarMB->GetInt();

				// Turn off motion blur and set anti-aliasing to fxaa
				CVarAA->Set(1);
				CVarMB->Set(0);

				Settings->Pauser = World->GetFirstPlayerController()->PlayerState;
			}
			// If we're unpausing
			else
			{
				// Reset motion blur and AA
				CVarAA->Set(PreviousAASetting);
				CVarMB->Set(PreviousMBSetting);

				Settings->Pauser = NULL;
			}				
		}		
	}
}

void UExtraFunctionalityLibrary::SetReplayPlaybackSpeed(const UObject* WorldContextObject, float NewReplaySpeed)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->GetWorldSettings()->DemoPlayTimeDilation = NewReplaySpeed;
	}
}

float UExtraFunctionalityLibrary::GetReplayPlaybackSpeed(const UObject * WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->GetWorldSettings()->DemoPlayTimeDilation;
	}
	return 0.0f;
}

float UExtraFunctionalityLibrary::GetCurrentReplayTotalTimeInSeconds(const UObject* WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (World->DemoNetDriver)
		{
			return World->DemoNetDriver->DemoTotalTime;
		}
	}
	return 0.0f;
}

float UExtraFunctionalityLibrary::GetCurrentReplayCurrentTimeInSeconds(const UObject* WorldContextObject)
{
	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (World->DemoNetDriver)
		{
			return World->DemoNetDriver->DemoCurrentTime;
		}
	}
	return 0.0f;
}

UPhysicalMaterial * UExtraFunctionalityLibrary::GetHitPhysMat(const FHitResult & Hit)
{
	return Hit.PhysMaterial.Get();
}

