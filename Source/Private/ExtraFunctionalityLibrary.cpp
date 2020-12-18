

#include "ExtraFunctionalityLibrary.h"
#include "AudioDeviceManager.h"
#include "AudioDevice.h"
#include "Components/SceneComponent.h"
#include "Components/Widget.h"
#include "ConfigCacheIni.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Engine/Console.h"
#include "Engine/Classes/GameFramework/GameMode.h"
#include "GenericPlatformMisc.h"
#include "ExtraMathLibrary.h"
#include "ExtraWidgetLibrary.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/FileManager.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NetworkReplayStreaming/Public/NetworkReplayStreaming.h"
#include "Engine/ObjectLibrary.h"
#include "Paths.h"
#include "Runtime/Engine/Classes/Engine/DemoNetDriver.h"
#include "Runtime/ApplicationCore/Public/HAL/PlatformApplicationMisc.h"
#include "UMG/Public/Components/CheckBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UserWidget.h"
#include "Widgets/SWidget.h"
#include "WidgetTree.h"

DEFINE_LOG_CATEGORY(LogExtraFunctionalityLibrary);

DECLARE_LOG_CATEGORY_EXTERN(LogCustomBlueprintCategory, Log, All);
DEFINE_LOG_CATEGORY(LogCustomBlueprintCategory);

#pragma region Platform Checks

void UExtraFunctionalityLibrary::SwitchOnPlatformType(EPlatformType& Result)
{
	Result = GetPlatformType();
}

bool UExtraFunctionalityLibrary::IsMobile()
{
#if (PLATFORM_ANDROID || PLATFORM_IOS)
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::PlatformRequiresGamepad()
{
	if (GetPlatformType() == EPlatformType::PT_Console)
	{
		return true;
	}
	return false;
}

FString UExtraFunctionalityLibrary::GetPlatformDeviceName()
{
	return FPlatformMisc::GetDefaultDeviceProfileName();
}

EPlatformType UExtraFunctionalityLibrary::GetPlatformType()
{
#if (PLATFORM_XBOXONE || PLATFORM_PS4 || PLATFORM_SWITCH || PLATFORM_TVOS)
		return EPlatformType::PT_Console;
#elif  (PLATFORM_MAC || PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_DESKTOP)
		return EPlatformType::PT_PC;
#elif (PLATFORM_ANDROID || PLATFORM_IOS)
		return EPlatformType::PT_Mobile;
#else
	return EPlatformType::PT_PC;
#endif
}

bool UExtraFunctionalityLibrary::DebugBuild()
{
#if UE_BUILD_DEBUG
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::DevelopmentBuild()
{
#if UE_BUILD_DEVELOPMENT
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::TestBuild()
{
#if UE_BUILD_TEST
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::ShippingBuild()
{
#if UE_BUILD_SHIPPING
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::WithEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::WindowsPlatform()
{
#if PLATFORM_WINDOWS
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::Windows32Platform()
{
#if PLATFORM_WINDOWS
#ifdef _WIN64
	return false;
#else
	return true;
#endif
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::Windows64Platform()
{
#if PLATFORM_WINDOWS
#ifdef _WIN64
	return true;
#else
	return false;
#endif
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::WindowsRtPlatform()
{
#ifdef PLATFORM_WINRT
#if PLATFORM_WINRT
	return true;
#else
	return false;
#endif
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::WindowsRtArmPlatform()
{
#ifdef PLATFORM_WINRT_ARM
#if PLATFORM_WINRT_ARM
	return true;
#else
	return false;
#endif
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::LinuxPlatform()
{
#if PLATFORM_LINUX
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::MacPlatform()
{
#if PLATFORM_MAC
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::Ps4Platform()
{
#if PLATFORM_PS4
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::XboxOnePlatform()
{
#if PLATFORM_XBOXONE
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::SwitchPlatform()
{
#if PLATFORM_SWITCH
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::AndroidPlatform()
{
#if PLATFORM_ANDROID
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::IosPlatform()
{
#if PLATFORM_IOS
	return true;
#else
	return false;
#endif
}

bool UExtraFunctionalityLibrary::DesktopPlatform()
{
#if PLATFORM_DESKTOP
	return true;
#else
	return false;
#endif
}

#pragma endregion

void UExtraFunctionalityLibrary::ForceCrash(const FString& CrashMessage)
{
#if !UE_BUILD_SHIPPING
	// Log the error a bunch to know where it happens exactly
	for (int index = 5; index-- > 0;)
	{
		UE_LOG(LogExtraFunctionalityLibrary, Fatal, TEXT("FORCING CRASH TO HAPPEN: %s"), *CrashMessage);
	}

	// Print that this was intended
	FDebug::AssertFailed("This crash was caused by UExtraFunctionalityLibrary::ForceCrash and was meant to happen.", __FILE__, __LINE__);
#endif
}

void UExtraFunctionalityLibrary::RequestExit(bool bForce)
{
	FGenericPlatformMisc::RequestExit(bForce);
}

void UExtraFunctionalityLibrary::SetGlobalVolume(UObject * WorldContextObject, float InAmount)
{
	if (GEngine)
	{
		if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			FAudioDeviceHandle Handle = World->GetAudioDevice();
			if(Handle.IsValid())
			{
				if (FAudioDevice* AudioDevice = Handle.GetAudioDevice())
				{
					AudioDevice->SetTransientMasterVolume(FMath::Max(InAmount, 0.0f));
				}
			}
		}
	}
}

bool UExtraFunctionalityLibrary::AreObjectsSameClass(UObject * A, UObject * B)
{	
	if (A && B)
	{
		return (A->GetClass() == B->GetClass());
	}
	return false;
}

bool UExtraFunctionalityLibrary::IsClassSameAs(UObject * A, UClass* ClassToCompare)
{
	if (!A || !ClassToCompare)
	{
		return false;
	}

	return (A->GetClass() == ClassToCompare);
}

void UExtraFunctionalityLibrary::GetAllLevels(UObject* WorldContextObject, TArray<ULevel*>& Levels)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	Levels = World->GetLevels();
}

UClass* UExtraFunctionalityLibrary::GetClassFromAssetPath(FString Path)
{
		TSubclassOf<UObject> AssetToCheck = NULL;

		// Attempt to load the asset normally
		{
			FString L, R;
			FString AssetName = Path;
			AssetName.Split(".", &L, &R, ESearchCase::IgnoreCase, ESearchDir::FromEnd);		

			if (AssetToCheck = LoadClass<UObject>(NULL, *AssetName, *Path))
			{
				return AssetToCheck.Get();
			}
		}
		// If we failed to load it normally then we try unconventional methods(cue Patrick looking menacing meme)

		// Try loading the object as an asset		
		UObject* Asset = StaticLoadObject(UObject::StaticClass(), nullptr, *Path);
		if (Asset)
		{									
			UBlueprint* Bp = Cast<UBlueprint>(Asset->GetClass());
			if (Bp)
			{				
				// Found asset
				AssetToCheck = Bp->GetClass();
			}
			else
			{
				UE_LOG(LogExtraFunctionalityLibrary, Warning, TEXT("Class not valid"));				
			}
		}
		else
		{
			UE_LOG(LogExtraFunctionalityLibrary, Warning, TEXT("Asset is null"));
		}

		return AssetToCheck.Get();
}

void UExtraFunctionalityLibrary::Conv_CollisionChannelsToObjectTypeQuerys(
	TArray<TEnumAsByte<ECollisionChannel>> Channels, TArray<TEnumAsByte<EObjectTypeQuery>>& ConvertedTypes)
{	
	ConvertedTypes.Reset();
	for (ECollisionChannel Channel : Channels)
	{
		ConvertedTypes.Add(UEngineTypes::ConvertToObjectType(Channel));
	}	
}

TEnumAsByte<EObjectTypeQuery> UExtraFunctionalityLibrary::Conv_CollisionChannelToObjectTypeQuery(
	TEnumAsByte<ECollisionChannel> Channel)
{
	return UEngineTypes::ConvertToObjectType(Channel);	
}

FString UExtraFunctionalityLibrary::GetLocalAppDataDirectory()
{
	return (FPaths::ConvertRelativePathToFull(FPaths::ProjectUserDir()));
}

TArray<FString> UExtraFunctionalityLibrary::GetSubDirectories(FString InDir, bool bDeepSearch)
{			
	// Turn it into a wildcard search
	FString FinalPath = InDir / TEXT("*");

	TArray<FString> FoundDir;
	// Gets finds the directories only
	IFileManager::Get().FindFiles(FoundDir, *FinalPath, false, true);

	// Loop through and add the directories path
	for (int index = FoundDir.Num(); index-- > 0;)
	{
		FoundDir[index] = (InDir + FoundDir[index] + "/");
	}

	// If we found directories and we're doing a deep search
	if (FoundDir.Num() > 0 && bDeepSearch)
	{
		// For each directory add each once's sub directories recursively
		for (int index = FoundDir.Num(); index-- > 0;)
		{
			// If that subdirectory has a subdirectory then it will keep going till an end is reached
			TArray<FString> SubDir = GetSubDirectories(FoundDir[index], true);
			FoundDir.Append(SubDir);
		}
	}
	return FoundDir;
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

bool UExtraFunctionalityLibrary::GetObjectsOf(TSubclassOf<UObject> InType, 
	TArray<UObject*>& OutputObjects, bool bIsBlueprintClass, 
	const FString & InFolder)
{
	// Valid check
	if (!InType || InFolder.IsEmpty())
	{
		UE_LOG(LogExtraFunctionalityLibrary, Warning, TEXT("Invalid parameters to get objects of."));
		return false;
	}

	// Get the objects in folder
	UObjectLibrary* Library = UObjectLibrary::CreateLibrary(InType, bIsBlueprintClass, GIsEditor);
	// Add the object library to asset registry to get the assets from it
	Library->AddToRoot();

	// Load the assets/blueprints
	if (bIsBlueprintClass)
	{
		Library->LoadBlueprintsFromPath(InFolder);
	}
	else
	{		
		
		Library->LoadAssetsFromPath(InFolder);
	}
	
	// Get the assets from the object libary to return them
	Library->GetObjects<UObject>(OutputObjects);
	Library->RemoveFromRoot();
	
	if (OutputObjects.Num() > 0)
	{
		// Log how many we found
		UE_LOG(LogExtraFunctionalityLibrary, Display, TEXT("Found %d %s's in directory: %s"), 
			OutputObjects.Num(), *InType->GetName(), *InFolder);
	}

	return (OutputObjects.Num() > 0);
}

FString UExtraFunctionalityLibrary::GetStringFromClipboard()
{
	FString FromClipboard;
	FPlatformApplicationMisc::ClipboardCopy(*FromClipboard);
	return FromClipboard;
}

void UExtraFunctionalityLibrary::CopyToClipboard(const FString& ToClipboard)
{
	FPlatformApplicationMisc::ClipboardCopy(*ToClipboard);
}

TArray<FString> UExtraFunctionalityLibrary::SortStrings(const TArray<FString> UnSortedStrings)
{
	TArray<FString> SortedStrings = UnSortedStrings;
	SortedStrings.StableSort();
	return SortedStrings;
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

bool UExtraFunctionalityLibrary::IsGamepadConnected()
{
	if (FSlateApplication::IsInitialized())
	{
		return FSlateApplication::Get().IsGamepadAttached();
	}
	return false;
}

bool UExtraFunctionalityLibrary::HasValidLocalPlayer(APlayerController * InController)
{
	if (InController)
	{		
		return (InController->GetLocalPlayer() != nullptr);
	}
	return false;
}

int UExtraFunctionalityLibrary::GetInputPriority(AActor* InActor)
{
	if (!InActor)
	{
		return 0;
	}
	
	return InActor->InputPriority;
}

bool UExtraFunctionalityLibrary::IsPawnInputEnabled(APawn * InPawn)
{
	if (InPawn)
	{
		return InPawn->InputEnabled();
	}
	return false;
}

void UExtraFunctionalityLibrary::SetInputPriority(AActor* InActor, int NewInputPriority)
{
	if (!InActor)
	{
		return;
	}
	
	InActor->InputPriority = NewInputPriority;
}

bool UExtraFunctionalityLibrary::TrySetPlayerInputEnabled(APawn * InPawn, const bool bIsEnabled)
{
	if (InPawn)
	{
		if (APlayerController* PC = Cast<APlayerController>(InPawn->GetController()))
		{			
			if (bIsEnabled)
			{
				InPawn->EnableInput(PC);
			}
			else
			{
				InPawn->DisableInput(PC);
			}
			return true;
		}
	}

	return false;
}

APlayerController* UExtraFunctionalityLibrary::TryGetPlayerControllerFromPawn(APawn * InPawn, EExtraSwitch & Result)
{
	Result = EExtraSwitch::OnFailed;
	if (InPawn)
	{
		if (APlayerController* PC = Cast<APlayerController>(InPawn->GetController()))
		{
			Result = EExtraSwitch::OnSucceeded;
			return PC;
		}
	}
	return nullptr;
}

void UExtraFunctionalityLibrary::AddForceToComponentBody(const UPrimitiveComponent * InComponent,
	const FVector& Force, const bool bAccelChange)
{
	if (!InComponent || !InComponent->IsSimulatingPhysics() || !InComponent->GetBodyInstance())
	{
		return;
	}
	InComponent->GetBodyInstance()->AddForce(Force, false, bAccelChange);
}

void UExtraFunctionalityLibrary::AddForceAtPositionToComponentBody(const UPrimitiveComponent * InComponent,
	const FVector& Force, const FVector& Position, const bool bLocalSpace)
{
	if (!InComponent || !InComponent->IsSimulatingPhysics() || !InComponent->GetBodyInstance())
	{
		return;
	}
	InComponent->GetBodyInstance()->AddForceAtPosition(Force, Position, false, bLocalSpace);	
}

void UExtraFunctionalityLibrary::AddImpulseAtPositionToComponentBody(const UPrimitiveComponent * InComponent,
	const FVector& Impulse, const FVector& Position)
{
	if (!InComponent || !InComponent->IsSimulatingPhysics() || !InComponent->GetBodyInstance())
	{
		return;
	}
	InComponent->GetBodyInstance()->AddImpulseAtPosition(Impulse, Position);
}

void UExtraFunctionalityLibrary::AddImpulseToComponentBody(const UPrimitiveComponent * InComponent,
	const FVector& Impulse, const bool bVelChange)
{
	if (!InComponent || !InComponent->IsSimulatingPhysics() || !InComponent->GetBodyInstance())
	{
		return;
	}
	InComponent->GetBodyInstance()->AddImpulse(Impulse, bVelChange);
}

void UExtraFunctionalityLibrary::AddTorqueInRadiansToComponentBody(const UPrimitiveComponent * InComponent,
	const FVector & Torque, const bool bAccelChange)
{
	if (!InComponent || !InComponent->IsSimulatingPhysics() || !InComponent->GetBodyInstance())
	{
		return;
	}
	InComponent->GetBodyInstance()->AddTorqueInRadians(Torque, false, bAccelChange);
}

void UExtraFunctionalityLibrary::AddAngularImpulseInRadiansToComponentBody(const UPrimitiveComponent * InComponent,
	const FVector & Impulse, const bool bVelChange)
{
	if (!InComponent || !InComponent->IsSimulatingPhysics() || !InComponent->GetBodyInstance())
	{
		return;
	}
	InComponent->GetBodyInstance()->AddAngularImpulseInRadians(Impulse, bVelChange);
}

void UExtraFunctionalityLibrary::SetLinearVelocityToComponentBody(const UPrimitiveComponent * InComponent,
	const FVector & NewVel, const bool bAddToCurrent)
{
	if (!InComponent || !InComponent->IsSimulatingPhysics() || !InComponent->GetBodyInstance())
	{
		return;
	}
	InComponent->GetBodyInstance()->SetLinearVelocity(NewVel, bAddToCurrent);
}

void UExtraFunctionalityLibrary::SetAngularVelocityInRadiansToComponentBody(const UPrimitiveComponent * InComponent,
	const FVector & NewAngVel, const bool bAddToCurrent)
{
	if (!InComponent || !InComponent->IsSimulatingPhysics() || !InComponent->GetBodyInstance())
	{
		return;
	}
	InComponent->GetBodyInstance()->SetAngularVelocityInRadians(NewAngVel, bAddToCurrent);
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

bool UExtraFunctionalityLibrary::IsRotatorZero(FRotator InRot)
{
	return InRot.IsZero();
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

int UExtraFunctionalityLibrary::GetNumberOfActorsOfType(const UObject * WorldContextObject, TSubclassOf<AActor> SearchClass)
{
	int FoundAmount = 0;

	if (UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{			
		// Loop through each actor in the world that is the same class as search class
		for (TActorIterator<AActor> ActorItr(World, SearchClass); ActorItr; ++ActorItr)
		{
			AActor* Actor = *ActorItr;
			if (!Actor->IsPendingKill())
			{				
				FoundAmount++;
			}
		}
	}

	return FoundAmount;
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

void UExtraFunctionalityLibrary::FindComponentOfClass(AActor * InActor, TSubclassOf<UActorComponent> ComponentClass, UActorComponent *& FoundComponent, EExtraSwitch & Result)
{
	Result = EExtraSwitch::OnFailed;
	FoundComponent = nullptr;
	if (ComponentClass && InActor)
	{
		if (UActorComponent* Comp = InActor->GetComponentByClass(ComponentClass))
		{
			FoundComponent = Comp;
			Result = EExtraSwitch::OnSucceeded;
		}
	}
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

float UExtraFunctionalityLibrary::GetFloatValue_RuntimeCurve(const FRuntimeFloatCurve& Curve, const float InTime)
{
	if (const FRichCurve* RichCurve = Curve.GetRichCurveConst())
	{		
		return RichCurve->Eval(InTime);
	}
	return 0.0f;
}

int UExtraFunctionalityLibrary::GetLastMaterialIndex(UPrimitiveComponent * Target)
{
	if (!Target)
	{
		return -1;
	}

	return (Target->GetNumMaterials() - 1);
}

TArray<UMaterialInterface*> UExtraFunctionalityLibrary::GetStaticMaterials(UStaticMesh * InMesh)
{
	if (!InMesh)
	{
		return TArray<UMaterialInterface*>();
	}

	TArray<UMaterialInterface*> Mats;
	for (FStaticMaterial SMat : InMesh->StaticMaterials)
	{
		Mats.Add(SMat.MaterialInterface);
	}
	return Mats;
}

const FKeyEvent UExtraFunctionalityLibrary::MakeKeyEvent(const int UserIndex, const FKey Key, const bool bIsRepeat)
{
	if (FSlateApplication::IsInitialized())
	{
		return FKeyEvent(
		Key, 
		FSlateApplication::Get().GetModifierKeys(), 
		UserIndex, 
		bIsRepeat, 
		0, 0); // Currently the slate application does not use character code or key code
	}
	return FKeyEvent(
		Key,
		FModifierKeysState(),
		UserIndex,
		bIsRepeat,
		0, 0); // Currently the slate application does not use character code or key code
}

const FPointerEvent UExtraFunctionalityLibrary::MakePointerEvent(const int UserIndex, const int PointerIndex, 
const FVector2D ScreenSpacePosition, const FVector2D LastScreenSpacePosition, 
const bool bPressed, const float WheelDelta)
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication& SlateApp = FSlateApplication::Get();
		
		return FPointerEvent(
		UserIndex, 
		PointerIndex, 
		ScreenSpacePosition, 
		LastScreenSpacePosition, 
		SlateApp.GetPressedMouseButtons(),
		bPressed ? EKeys::LeftMouseButton : EKeys::Invalid,
		WheelDelta,
		SlateApp.GetPlatformApplication()->GetModifierKeys()
		);
	}

	return FPointerEvent(
	UserIndex, 
	PointerIndex, 
	ScreenSpacePosition, 
	LastScreenSpacePosition,
	TSet<FKey>(),
	bPressed ? EKeys::LeftMouseButton : EKeys::Invalid,
	WheelDelta,
	FModifierKeysState()
	);
}

const FMotionEvent UExtraFunctionalityLibrary::MakeMotionEvent(const int UserIndex, 
const FVector Tilt, const FVector RotationRate, const FVector Gravity, const FVector Acceleration)
{
	return FMotionEvent(UserIndex, Tilt, RotationRate, Gravity, Acceleration);
}

void UExtraFunctionalityLibrary::ClearInputMappings(UInputSettings * const InSettings, bool bForceRebuildKeymaps, bool bSaveKeyMappings)
{
	// Dont continue if invalid
	if (!InSettings)
	{
		return;
	}

	// Clear action mappings
	for (const FInputActionKeyMapping& Action : InSettings->GetActionMappings())
	{
		InSettings->RemoveActionMapping(Action, false);
	}
	// Clear axis mappings
	for (const FInputAxisKeyMapping& Axis : InSettings->GetAxisMappings())
	{
		InSettings->RemoveAxisMapping(Axis, false);
	}

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

bool UExtraFunctionalityLibrary::FindSceneComponentByName(AActor * ActorToSearchIn,
	const FString & CompName, USceneComponent *& FoundComp)
{
	if (ActorToSearchIn && !CompName.IsEmpty())
	{
		for (UActorComponent* Comp : ActorToSearchIn->GetComponents())
		{
			if (USceneComponent* SComp = Cast<USceneComponent>(Comp))
			{
				if (GetNameSafe(SComp) == CompName)
				{
					FoundComp = SComp;
					return true;
				}
			}
		}
	}
	return false;
}

bool UExtraFunctionalityLibrary::FindActorComponentByName(AActor* ActorToSearchIn,
	const FString & CompName, UActorComponent *& FoundComp)
{
	if (ActorToSearchIn && !CompName.IsEmpty())
	{
		for (UActorComponent* Comp : ActorToSearchIn->GetComponents())
		{
			if (GetNameSafe(Comp) == CompName)
			{
				FoundComp = Comp;
				return true;
			}
		}
	}
	return false;
}

UPrimitiveComponent * UExtraFunctionalityLibrary::GetClosestComponentToPoint(TArray<UPrimitiveComponent*> Comps, 
	FVector Point, bool Inverse)
{
	UPrimitiveComponent* ClosestComp = nullptr;
	float ClosestDistance = (Inverse) ? 0.0f : MAX_flt;

	for (UPrimitiveComponent* Comp : Comps)
	{
		float CurrentDistance = (Comp->GetComponentLocation() - Point).SizeSquared();
		if (Inverse)
		{
			if (CurrentDistance > ClosestDistance)
			{
				ClosestDistance = CurrentDistance;
				ClosestComp = Comp;
			}
		}
		else
		{
			if (CurrentDistance < ClosestDistance)
			{
				ClosestDistance = CurrentDistance;
				ClosestComp = Comp;
			}
		}
	}

	return ClosestComp;
}

AActor * UExtraFunctionalityLibrary::GetClosestActorToPoint(TArray<AActor*> Actors, 
	FVector Point, bool Inverse)
{
	AActor* ClosestActor = nullptr;
	float ClosestDistance = (Inverse) ? 0.0f : MAX_flt;

	for (AActor* Actor : Actors)
	{
		float CurrentDistance = (Actor->GetActorLocation() - Point).SizeSquared();

		if (Inverse)
		{
			if (CurrentDistance > ClosestDistance)
			{
				ClosestDistance = CurrentDistance;
				ClosestActor = Actor;
			}
		}
		else
		{
			if (CurrentDistance < ClosestDistance)
			{
				ClosestDistance = CurrentDistance;
				ClosestActor = Actor;
			}
		}
	}

	return ClosestActor;
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

float UExtraFunctionalityLibrary::SetSplineMeshRelativeRoll(USplineComponent* SplineComp, 
	const FRotator RelativeRotation, const float DistanceAlongSpline, const bool bReturnInRadians)
{
	if (!SplineComp)
	{
		return 0.0f;
	}
	const ESplineCoordinateSpace::Type Coordinate = ESplineCoordinateSpace::Local;
	const FVector Forward = RelativeRotation.UnrotateVector(SplineComp->GetDirectionAtDistanceAlongSpline(DistanceAlongSpline, Coordinate)).GetSafeNormal();
	const FVector Right = RelativeRotation.UnrotateVector(SplineComp->GetRightVectorAtDistanceAlongSpline(DistanceAlongSpline, Coordinate)).GetSafeNormal();
	const FVector Up = RelativeRotation.UnrotateVector(SplineComp->GetUpVectorAtDistanceAlongSpline(DistanceAlongSpline, Coordinate)).GetSafeNormal();

	// Make rotation from axes, then get the roll from it
	const float Roll = FMatrix(Forward, Right, Up, FVector::ZeroVector).Rotator().Roll;
	return (bReturnInRadians ? FMath::DegreesToRadians(Roll) : Roll);
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

void UExtraFunctionalityLibrary::SnapSingleSplinePointToGround(USplineComponent * SplineComp,
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

TArray<USplineMeshComponent*> UExtraFunctionalityLibrary::BuildSplineMeshesAlongSpline(
	USplineComponent* SplineComp, FExtraSplineConstructionInfo ConstructionInfo)
{
	if (!SplineComp || !ConstructionInfo.SplineMesh)
	{
		return TArray<USplineMeshComponent*>();
	}
	TArray<USplineMeshComponent*> SplineMeshes;

	if (UWorld* const World = SplineComp->GetWorld())
	{
		// Base the up vector for each spline point
		for (int32 index = 0; index < (SplineComp->GetNumberOfSplinePoints() - 1); index++)
		{
			if (!IsValidSplinePoint(SplineComp, index))
			{
				break;
			}			
			SplineComp->SetUpVectorAtSplinePoint(index, 
				FRotationMatrix(
					SplineComp->GetRotationAtSplinePoint(index, ESplineCoordinateSpace::World)).GetScaledAxis(EAxis::Z), 
				ESplineCoordinateSpace::World);
		}

		const int32 SplineEndAmount = (FMath::TruncToInt(SplineComp->GetSplineLength() / ConstructionInfo.SplineTileLength) + 1);
		const ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::Local;		

		// Construct the spline meshes
		for (int32 index = 0; index < SplineEndAmount; index++)
		{			
			// Setup the values to set for the spline meshes
			const float CurrentDistance = index * ConstructionInfo.SplineTileLength;
			const float NextDistance = FMath::Clamp((index + 1) * ConstructionInfo.SplineTileLength, 0.0f, SplineComp->GetSplineLength());
			const float MidPointSplineDistance = (CurrentDistance + NextDistance) * 0.5f;
			const float CurrentTileLength = NextDistance - CurrentDistance;

			// Create component and set default values for it
			USplineMeshComponent* MeshComp = NewObject<USplineMeshComponent>(SplineComp);
			MeshComp->SetMobility(ConstructionInfo.Mobility);
			if (MeshComp->GetAttachParent() != SplineComp)
			{				
				MeshComp->SetupAttachment(SplineComp);
			}
			MeshComp->SetRelativeTransform(FTransform());
			MeshComp->SetStartScale(ConstructionInfo.StartScale, false);
			MeshComp->SetEndScale(ConstructionInfo.EndScale, false);
			MeshComp->SetCanEverAffectNavigation(ConstructionInfo.bAffectNavigation);
			MeshComp->SetGenerateOverlapEvents(ConstructionInfo.bGenerateOverlapEvents);
			MeshComp->SetCollisionObjectType(UEngineTypes::ConvertToCollisionChannel(ConstructionInfo.ObjectType));
			MeshComp->SetCollisionEnabled(ConstructionInfo.CollisionEnabled);
			MeshComp->SetForwardAxis(ConstructionInfo.ForwardAxis, false);
			MeshComp->SetStaticMesh(ConstructionInfo.SplineMesh);
			
			// Apply the materials
			{
				TArray<UMaterialInterface*> SplineMaterials = (ConstructionInfo.OptionalMaterials.Num() > 0) ?
					ConstructionInfo.OptionalMaterials : GetStaticMaterials(ConstructionInfo.SplineMesh);
			
				if (SplineMaterials.Num() > 0)
				{
					for (int MatIndex = SplineMaterials.Num(); MatIndex-- > 0;)
					{
						// Valid check
						if (SplineMaterials[MatIndex])
						{
							MeshComp->SetMaterial(MatIndex, SplineMaterials[MatIndex]);
						}
					}
				}
			}
			MeshComp->SetSplineUpDir(
				FRotationMatrix(
					SplineComp->GetRotationAtDistanceAlongSpline(MidPointSplineDistance, CoordinateSpace)).GetScaledAxis(EAxis::Z), 
				false);

			// Set start & end
			{
				const FVector SplineStartTangent = SplineComp->GetTangentAtDistanceAlongSpline(CurrentDistance, CoordinateSpace);
				const FVector StartTangent = SplineStartTangent.GetSafeNormal() * FMath::Min(SplineStartTangent.Size(), CurrentTileLength);

				const FVector SplineEndTangent = SplineComp->GetTangentAtDistanceAlongSpline(NextDistance, CoordinateSpace);
				const FVector EndTangent = SplineEndTangent.GetSafeNormal() * FMath::Min(SplineEndTangent.Size(), CurrentTileLength);

				MeshComp->SetStartAndEnd(SplineComp->GetLocationAtDistanceAlongSpline(CurrentDistance, CoordinateSpace), StartTangent,
					SplineComp->GetLocationAtDistanceAlongSpline(NextDistance, CoordinateSpace), EndTangent, false);
			}
			
			// Set starting roll
			{
				const float CorrectedRoll = UExtraFunctionalityLibrary::SetSplineMeshRelativeRoll(SplineComp,
					SplineComp->GetRotationAtDistanceAlongSpline(MidPointSplineDistance, CoordinateSpace), CurrentDistance, true);
				MeshComp->SetStartRoll(CorrectedRoll, false);
			}

			// Set end roll
			{
				const float CorrectedRoll = UExtraFunctionalityLibrary::SetSplineMeshRelativeRoll(SplineComp,
					SplineComp->GetRotationAtDistanceAlongSpline(MidPointSplineDistance, CoordinateSpace), NextDistance, true);
				MeshComp->SetEndRoll(CorrectedRoll, false);
			}

			MeshComp->UpdateMesh();
			MeshComp->RegisterComponentWithWorld(World);
			SplineMeshes.Add(MeshComp);

			if (ConstructionInfo.bDebugMode)
			{				
				const FVector StartLoc = (SplineComp->GetLocationAtDistanceAlongSpline(MidPointSplineDistance, ESplineCoordinateSpace::World));
				const FRotator BaseRotation = (SplineComp->GetRotationAtDistanceAlongSpline(MidPointSplineDistance, ESplineCoordinateSpace::World));
				const float Length = ConstructionInfo.ArrowLength;
				const float Size = ConstructionInfo.ArrowSize;
				const float Thickness = ConstructionInfo.ArrowThickness;
				const float DisplayTime = ConstructionInfo.DebugTime;

				// X
				DrawDebugDirectionalArrow(World, StartLoc, StartLoc + (FRotationMatrix(BaseRotation).GetScaledAxis(EAxis::X) * Length),
					Size, FColor::Red, false, DisplayTime, SDPG_World, Thickness);
				// Y
				DrawDebugDirectionalArrow(World, StartLoc, StartLoc + (FRotationMatrix(BaseRotation).GetScaledAxis(EAxis::Y) * Length),
					Size, FColor::Green, false, DisplayTime, SDPG_World, Thickness);
				// Z
				DrawDebugDirectionalArrow(World, StartLoc, StartLoc + (FRotationMatrix(BaseRotation).GetScaledAxis(EAxis::Z) * Length),
					Size, FColor::Blue, false, DisplayTime, SDPG_World, Thickness);
			}
		}
	}
	
	return SplineMeshes;
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
		return World->DemoNetDriver != nullptr;
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
			return Settings->GetPauserPlayerState() != nullptr;			
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

		const bool IsPaused = false;// Settings->Pauser;
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

				Settings->SetPauserPlayerState(World->GetFirstPlayerController()->PlayerState);				
			}
			// If we're unpausing
			else
			{
				// Reset motion blur and AA
				CVarAA->Set(PreviousAASetting);
				CVarMB->Set(PreviousMBSetting);
				
				Settings->SetPauserPlayerState(NULL);
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

