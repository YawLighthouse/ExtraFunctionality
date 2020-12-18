

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h" // This is for the ESplineCoordinateSpace
#include "GameFramework/PlayerInput.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlateCore/Public/Styling/SlateTypes.h"
#include "Runtime/Engine/Classes/Engine/NetConnection.h"
#include "Runtime/Sockets/Public/IPAddress.h"
#include "ExtraDataTypes.h"
#include "ExtraFunctionalityLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExtraFunctionalityLibrary, Log, All);

class AGameMode;

class USplineMeshComponent;

class UCheckBox;
class UUserWidget;
class UWidget;
class UPanelWidget;
class UTextBlock;

class UInputSettings;
class UStaticMesh;

UCLASS()
class EXTRAFUNCTIONALITY_API UExtraFunctionalityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#pragma region Platform Stuff


	/** Shorthand function for checking what platform type this application is currently running on. */
	UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Platform", 
		meta = (Keywords = "type platform", ExpandEnumAsExecs = "Result"))
		static void SwitchOnPlatformType(EPlatformType& Result);

	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform")
		static bool IsMobile();

	/** Returns true if the game is running on a platform that requires a gamepad. (Desktop and Mobile do not require it but it is recommended to have support for it on those platforms) */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform")
	static bool PlatformRequiresGamepad();

	/** Returns the platform device's name */
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform")
		static FString GetPlatformDeviceName();

	/** 
	* Returns the current platform type.
	* Defaults to Desktop if on a platform that isn't accounted for.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (Keywords = "type platform"))
	static EPlatformType GetPlatformType();

	/**
	* Returns true if this is a debug build (UE_BUILD_DEBUG), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform|Build", 
		meta = (DisplayName = "Is Debug Build", 
			Keywords = "compile export mode version type build platform"))
		static bool DebugBuild();

	/**
	* Returns true if this is a debug build (UE_BUILD_DEVELOPMENT), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform|Build", 
		meta = (DisplayName = "Is Development Build", 
			Keywords = "compile export mode version type build platform"))
		static bool DevelopmentBuild();

	/**
	* Returns true if this is a debug build (UE_BUILD_TEST), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform|Build", 
		meta = (DisplayName = "Is Test Build", 
			Keywords = "compile export mode version type build platform"))
		static bool TestBuild();

	/**
	* Returns true if this is a debug build (UE_BUILD_SHIPPING), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform|Build", 
		meta = (DisplayName = "Is Shipping Build", 
			Keywords = "compile export mode version type build platform"))
		static bool ShippingBuild();

	/**
	* Returns true if this is inside the editor (WITH_EDITOR), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform|Build", 
		meta = (DisplayName = "With Editor", 
			Keywords = "compile export mode version type build platform in inside using"))
		static bool WithEditor();

	/**
	* Returns true if this is the Windows platform (PLATFORM_WINDOWS), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Windows Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool WindowsPlatform();

	/**
	* Returns true if this is the Windows platform running on 32 bit (PLATFORM_WINDOWS and not _WIN64), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Windows Platform (32 bit)", 
			Keywords = "compile export mode version type build platform"))
		static bool Windows32Platform();

	/**
	* Returns true if this is the Windows platform running on 64 bit (PLATFORM_WINDOWS and _WIN64), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Windows Platform (64 bit)", 
			Keywords = "compile export mode version type build platform"))
		static bool Windows64Platform();

	/**
	* Returns true if this is the Windows RT platform (PLATFORM_WINRT), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Windows Rt Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool WindowsRtPlatform();

	/**
	* Returns true if this is the Windows RT ARM platform (PLATFORM_WINRT_ARM), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Windows Rt Arm Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool WindowsRtArmPlatform();

	/**
	* Returns true if this is the Linux platform (PLATFORM_LINUX), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Linux Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool LinuxPlatform();

	/**
	* Returns true if this is the Mac platform (PLATFORM_MAC), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Mac Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool MacPlatform();

	/**
	* Returns true if this is the PS4 platform (PLATFORM_PS4), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Ps4 Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool Ps4Platform();

	/**
	* Returns true if this is the Xbox One platform (PLATFORM_XBOXONE), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Xbox One Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool XboxOnePlatform();

	/**
	* Returns true if this is the Switch platform (PLATFORM_SWITCH), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Switch Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool SwitchPlatform();

	/**
	* Returns true if this is the Android platform (PLATFORM_ANDROID), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Android Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool AndroidPlatform();

	/**
	* Returns true if this is the IOS platform (PLATFORM_IOS), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Ios Platform", 
			Keywords = "compile export mode version type build platform"))
		static bool IosPlatform();

	/**
	* Returns true if this is a desktop (PLATFORM_DESKTOP), returns false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Platform", 
		meta = (DisplayName = "Is Desktop", 
			Keywords = "compile export mode version type build platform"))
		static bool DesktopPlatform();

#pragma endregion

		/** 
		* Forces the game to crash, this is intended for debugging purposes and should not be used in packaged builds. 
		* This will print a log message before the crash as a heads up. 
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Test", meta = (DevelopmentOnly))
		static void ForceCrash(const FString& CrashMessage = "Generic Testing Purposes");

		/** Requests this application to close itself. */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library",
			meta = (DisplayName = "Shutdown", Keywords = "Shutdown"))
		static void RequestExit(bool bForce);

		/** Handles updating the global audio device's volume ingame. DO NOT USE FOR SETTINGS */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library", meta = (WorldContext = "WorldContextObject"))
		static void SetGlobalVolume(UObject* WorldContextObject, float InAmount);

		/** Returns true if both objects are the same class, false if otherwise(or if either or null) */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static bool AreObjectsSameClass(UObject* A, UObject* B);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
			static bool IsClassSameAs(UObject* A, UClass* ClassToCompare);

		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library", 
			meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, 
				DisplayName = "Get All Levels", Keywords = "Get All Levels"))
		static void GetAllLevels(UObject* WorldContextObject, TArray<ULevel*>& Levels);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static UClass* GetClassFromAssetPath(FString Path);

		/** Converts a linear color to a slate color */		
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Conversions", 
			meta = (DisplayName = "ToSlateColor (Linear Color)",
			CompactNodeTitle = "->", BlueprintAutocast))
		static FSlateColor Conv_LinearColorToSlateColor(FLinearColor InLinearColor) { return FSlateColor(InLinearColor); }

		/** Converts an array of collision channel's to object type query's */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Conversions",
			meta = (DisplayName = "ToObjectTypeQuery's (Collision Channel's)",
				CompactNodeTitle = "->"))
		static void Conv_CollisionChannelsToObjectTypeQuerys(
			TArray<TEnumAsByte<ECollisionChannel>> Channels, TArray<TEnumAsByte<EObjectTypeQuery>>& ConvertedTypes);

		/** Converts an collision channel to object type query */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Conversions",
			meta = (DisplayName = "ToObjectTypeQuery (Collision Channel)",
				CompactNodeTitle = "->", BlueprintAutocast))
		static TEnumAsByte<EObjectTypeQuery> Conv_CollisionChannelToObjectTypeQuery(TEnumAsByte<ECollisionChannel> Channel);

#pragma region Directory Stuff

		/** Handles getting the local app data directory, in editor will return the entire project directory(IF USING DELETE DIRECTORY WITH THIS ONE, MAKE SURE ITS NOT IN EDITOR) */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Files")
		static FString GetLocalAppDataDirectory();		

		/** 
		* Gets the sub directories within the inputted directory, 
		* *NOTE* this can cause hitches!
		* @param bDeepSearch Recursively loop through each directory searching for them, will increase hitch time.
		*/
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Files")
		static TArray<FString> GetSubDirectories(FString InDir, bool bDeepSearch);

		/** Deletes the inputted directory if found. */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Files")
		static bool DeleteDirectory(FString InDir);

		/** Deletes the inputted file at the directory if found. */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Files")
		static bool DeleteFile(FString InFileDir);		

		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Files", meta = 
			(UnsafeDuringActorConstruction = "true", 
			DeterminesOutputType = "InType", DynamicOutputParam = "OutputObjects"))
		static bool GetObjectsOf(TSubclassOf<UObject> InType, 
			TArray<UObject*>& OutputObjects, bool bIsBlueprintClass,
			const FString& InFolder = "/Game/");

#pragma endregion

#pragma region String Stuff

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Strings")
		static FString GetStringFromClipboard();

		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Strings")
		static void CopyToClipboard(const FString& ToClipboard);

		/** Handles sorting an array of strings to be alphabetical */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Strings")
		static TArray<FString> SortStrings(const TArray<FString> UnSortedStrings);

		/** 
		* Increases the verbosity of the inputted message for printing a string in blueprint to be either a warning or error by adding a blueprint friendly prefix. 
		* Example: printing out "This is a test" as a warning will print in the log "warning:This is a test" and the prefix for an error would be "error:"
		* @param bIsError If true will return a version of the message that will be an error, false will return a version of the message that will be a warning.
		*/
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|String")
		static FString IncreaseVerbosityOfMessage(FString InMessage, bool bIsError=false);

		/**
		 * Prints a string to the log with a custom category type incase you want to search for that, and optionally, to the screen
		 * If Print To Log is true, it will be visible in the Output Log window.  Otherwise it will be logged only as 'Verbose', so it generally won't show up.		
		 * So a example log would be: [LogCustomBlueprintCategory: Custom Category:: Hello]
		 *
		 * @param	InString		The string to log out
		 * @param	InCategory		The custom category that gets appended ontu the LogCustomBlueprintCategory category. If empty will use "EmptyCustomCategory".
		 * @param	bPrintToScreen	Whether or not to print the output to the screen. Will not print to the screen in shipping or test builds.
		 * @param	bPrintToLog		Whether or not to print the output to the log		 
		 * @param	TextColor		Whether or not to print the output to the log
		 * @param	Duration		The display duration (if Print to Screen is True). Using negative number will result in loading the duration time from the config.
		 */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|String",
			meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, 
				Keywords = "log print", AdvancedDisplay = "3", DevelopmentOnly))
			static void PrintStringWithCategory(UObject* WorldContextObject,
				const FString& InString = FString(TEXT("Hello")),
				FString InCategory = FString(TEXT("Custom Category")),
				bool bPrintToScreen = true, bool bPrintToLog = true,
				FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), float Duration = 2.f);


		/** Returns a non destructive copy of SourceString with all underscores replaced with a space. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|String")
		static FString ReplaceUnderscore(FString SourceString);

		/** Returns a non destructive copy of SourceString with all spaces removed. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|String")
		static FString RemoveSpaces(FString SourceString);

		/** Returns true if SourceString is empty. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|String",
			meta = (DisplayName = "Is Empty (String)", BlueprintAutocast))
			static bool IsEmpty_String(const FString& SourceString) { return SourceString.IsEmpty(); }

#pragma endregion

		/** Returns true if SourceName is empty. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Name",
			meta = (DisplayName = "Is Empty (Name)", BlueprintAutocast))
			static bool IsEmpty_Name(const FName& SourceName) { return SourceName == NAME_None; }

#pragma region IP stuff

		/** 
		* Should only occur on the server(regardless of dedicated or listen servers), gets the currently connected client IP's addresses
		* @return The currently connected IP's Addresses
		*/
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Networking", 
			meta = (WorldContext = "WorldContextObject", DisplayName = "Get Connected Client IP's", Keywords = "ip"))
		static TArray<FString> GetConnectedClientIPs(const UObject* WorldContextObject);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Networking", meta = (Keywords = "ip"))
		static FString GetServerIpFromGamemode(AGameMode* InGamemode);

		/**
		* Should only occur on clients(listen server will not count), gets the server's IP address
		* @return The server's IP Address
		*/
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Networking", 
			meta = (WorldContext = "WorldContextObject", DisplayName = "Get Server IP", Keywords = "ip"))
			static FString GetServerIP(const UObject* WorldContextObject);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Networking",
			meta = (DisplayName = "Get Player's IP", Keywords = "ip"))
		static FString GetPlayerIP(const APlayerController* InPlayer);

#pragma endregion

#pragma region Player Stuff

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static bool IsGamepadConnected();				

		/** 
		* Returns true if the local player has been set for this player controller. 
		* Returns false for invalid player controller, or local player hasn't been set(suggest overriding function ReceivePlayer in player controller which happens after local player is set)
		*/
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static bool HasValidLocalPlayer(APlayerController* InController);

		/** Returns the input priority of the inputted actor, returns zero if actor is invalid. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static int GetInputPriority(AActor* InActor);

		/** 
		* Returns true if the pawn's input is enabled, false if otherwise. 
		* To check for generic actors, it needs to be setup in the player controller by checking if it contains that actor's input component in the CurrentInputStack array in C++(because that variable is protected atm).
		*/
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static bool IsPawnInputEnabled(APawn* InPawn);

		/** Handles setting the input priority for the inputted actor. */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library")
		static void SetInputPriority(AActor* InActor, int NewInputPriority);

		/** 
		* Shorthand function for easily enabling/disabling input for a pawn if its possessed by a player controller. 
		* @return Returns if successfully set or not.
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library")
		static bool TrySetPlayerInputEnabled(APawn* InPawn, const bool bIsEnabled);				

		/** Attempts to get the player controller from the inputted pawn. */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library", meta = (ExpandEnumAsExecs = "Result"))
		static APlayerController* TryGetPlayerControllerFromPawn(APawn* InPawn, EExtraSwitch &Result);

#pragma endregion

#pragma region Physics Stuff

		/** Adds force to a components rigid body instance(intended for when substepping) */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Physics")
		static void AddForceToComponentBody(const UPrimitiveComponent* InComponent,
			const FVector& Force, const bool bAccelChange);

		/** 
		* Adds force at a position to a components rigid body instance(intended for when substepping) 
		* @param bLocalSpace If true applies the force in local space of the object, false will apply in world space.
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Physics")
		static void AddForceAtPositionToComponentBody(const UPrimitiveComponent* InComponent,
			const FVector& Force, const FVector& Position, const bool bLocalSpace);

		/** Adds impulse at position to a components rigid body instance(intended for when substepping) */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Physics")
			static void AddImpulseAtPositionToComponentBody(const UPrimitiveComponent* InComponent,
				const FVector& Impulse, const FVector& Position);

		/** Adds impulse to a components rigid body instance(intended for when substepping) */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Physics")
			static void AddImpulseToComponentBody(const UPrimitiveComponent* InComponent,
				const FVector& Impulse, const bool bVelChange);

		/** Adds torque(in radians) to a components rigid body instance(intended for when substepping) */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Physics")
		static void AddTorqueInRadiansToComponentBody(const UPrimitiveComponent* InComponent,
			const FVector& Torque, const bool bAccelChange);

		/** Adds an angular impulse(in radians) to a components rigid body instance(intended for when substepping) */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Physics")
			static void AddAngularImpulseInRadiansToComponentBody(const UPrimitiveComponent* InComponent,
				const FVector& Impulse, const bool bVelChange);

		/** Handles setting the velocity of a components rigid body instance(intended for when substepping) */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Physics")
		static void SetLinearVelocityToComponentBody(const UPrimitiveComponent* InComponent,
			const FVector& NewVel, const bool bAddToCurrent);

		/** Handles setting the angular velocity of a components rigid body instance(intended for when substepping) */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Physics")
		static void SetAngularVelocityInRadiansToComponentBody(const UPrimitiveComponent* InComponent,
			const FVector& NewAngVel, const bool bAddToCurrent);

#pragma endregion

		/** Returns the relative location of a socket. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Transformation")
		static FVector GetSocketRelativeLocation(USceneComponent* Target, FName InSocketName);

		/** Returns the relative rotation of a socket. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Transformation")
		static FRotator GetSocketRelativeRotation(USceneComponent* Target, FName InSocketName);

		/** Returns true if the inputted rotator is equal to zero. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Transformation")
		static bool IsRotatorZero(FRotator InRot);

		/** Marks the inputted component's render state dirty in all aspects possible. */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library")
		static bool MarkRenderDirty(USceneComponent* InComp);

		/** Checks if all objects inputted into the array are valid. False if empty, True if all objects are valid. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library", meta = (Keywords = "valid isvalid"))
			static bool AreObjectsValid(TArray<UObject*> Objects);

		/** Returns true if any actors are overlapping this component, will return false if component is invalid. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
			static bool IsOverlappingAnyActors(UPrimitiveComponent* InComp, bool bExcludeSelf = true);

		/** Returns the amount of actors in the world that are of type SearchClass */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library",
			meta = (WorldContext = "WorldContextObject"))
			static int GetNumberOfActorsOfType(const UObject* WorldContextObject, TSubclassOf<AActor> SearchClass);

		/** 
		* Attempts to return the first valid instance of an actor with SearchClass.) 
		* @return Returns true if found first valid instance of actor. False if otherwise.
		*/
		UE_DEPRECATED(4.23, "This function is deprecated. Please use GetActorOfClass instead.")
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library",
			meta = (WorldContext = "WorldContextObject", 
				DeterminesOutputType = "SearchClass", DynamicOutputParam = "FoundActor", 
				Keywords = "first"))
		static bool FindFirstInstanceOfActorType(const UObject* WorldContextObject, 
			TSubclassOf<AActor> SearchClass, AActor*& FoundActor);

		/** Attempts to find the component of class. */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library", 
			meta = (DeterminesOutputType = "ComponentClass", DynamicOutputParam = "FoundComponent", ExpandEnumAsExecs = "Result"))
			static void FindComponentOfClass(AActor* InActor, 
				TSubclassOf<UActorComponent> ComponentClass, UActorComponent*& FoundComponent, EExtraSwitch &Result);

		/** Marks multiple inputted component's render state dirty. */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library", meta = (DisplayName = "Mark Render Dirty (Array)"))
		static void MarkRenderDity_Comps(TArray<USceneComponent*> InComps);

		/** 
		* Returns axis values based on if inputs are pressed/released within InPlayerController.
		* Having to specify each directional input allows for agnostic values.
		* EX: UpDirectional = Up on DPad is pressed = 1 on Y, DownDirectional = Down on DPad is pressed = -1 on Y. And same for Left(-1 on X) and Right(1 on X).
		* @param UpDirectional Will be positive Y value when pressed.
		* @param DownDirectional Will be negative Y value when pressed.
		* @param RightDirectional Will be positive X value when pressed.
		* @param LeftDirectional Will be negative X value when pressed.
		*/
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static FVector2D GetDirectionalInputsFromDirectionalKeys(const APlayerController* InPlayerController, const FKey UpDirectional, const FKey DownDirectional,
			const FKey RightDirectional, const FKey LeftDirectional);

		/**
		* Returns a axis value based on if inputs are pressed/released within InPlayerController.
		* Having to specify each input allows for agnostic values.
		*/
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static float GetAxisValueOfInputs(const APlayerController* InPlayerController, 
			const FKey PositiveKey, const FKey NegativeKey);

		/** Returns the float value at the inputted time inside a runtime curve */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library", meta = (DisplayName = "Get Float Value (Runtime Curve)"))
		static float GetFloatValue_RuntimeCurve(UPARAM(ref) const FRuntimeFloatCurve& Curve, const float InTime);

		/** Shorthand function for getting the last index of material array in Primitive Component. RETURNS -1 IF TARGET IS INVALID */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library", meta = (CompactNodeTitle = "LAST INDEX"))
		static int GetLastMaterialIndex(UPrimitiveComponent* Target);

		/** Returns the static(default) material instances for the inputted static mesh. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
		static TArray<UMaterialInterface*> GetStaticMaterials(UStaticMesh* InMesh);

#pragma region Input Stuff

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Input")
		static const FKeyEvent MakeKeyEvent(const int UserIndex, const FKey Key, const bool bIsRepeat);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Input")
		static const FPointerEvent MakePointerEvent(const int UserIndex, const int PointerIndex, 
		const FVector2D ScreenSpacePosition, const FVector2D LastScreenSpacePosition, 
		const bool bPressed, const float WheelDelta);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Input")
		static const FMotionEvent MakeMotionEvent(const int UserIndex, 
		const FVector Tilt, const FVector RotationRate, const FVector Gravity, const FVector Acceleration);

		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Input")
		static void ClearInputMappings(UInputSettings* const InSettings, bool bForceRebuildKeymaps, bool bSaveKeyMappings);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Input", meta = (DisplayName = "Equal (Input Axis Key Mapping)", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool Equals_InputAxisKeyMapping(FInputAxisKeyMapping A, FInputAxisKeyMapping B, bool bNameOnly);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Input", meta = (DisplayName = "Equal (Input Action Key Mapping)", CompactNodeTitle = "==", Keywords = "== equal"))
		static bool Equals_InputActionKeyMapping(FInputActionKeyMapping A, FInputActionKeyMapping B, bool bNameOnly);

#pragma endregion

		/** Searches for an scene component that's name is matching the inputted search name. Returns true if success, and false if otherwise. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
			static bool FindSceneComponentByName(AActor* ActorToSearchIn,
				const FString& CompName, USceneComponent*& FoundComp);

		/** Searches for an actor component that's name is matching the inputted search name. Returns true if success, and false if otherwise. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
			static bool FindActorComponentByName(AActor* ActorToSearchIn, 
				const FString& CompName, UActorComponent*& FoundComp);

		/** Gets the closest component to the inputted point(in world space) from the inputted array of components */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
			static UPrimitiveComponent* GetClosestComponentToPoint(TArray<UPrimitiveComponent*> Comps, 
				FVector Point, bool Inverse);

		/** Gets the closest actor to the inputted point(in world space) from the inputted array of actors */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library")
			static AActor* GetClosestActorToPoint(TArray<AActor*> Actors, FVector Point, bool Inverse);

		/**
		* Draws coordinate arrows at InComponent's world location for DebugDisplayTime's seconds if InComponent is valid.
		* @param ForwardColor Forward Vector of InComponent(X).
		* @param RightColor Right Vector of InComponent(Y).
		* @param UpColor Up Vector of InComponent(Z).
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Debug", 
			meta = (DevelopmentOnly, ArrowLength = "200.0", ArrowSize = "100.0", ArrowThickness = "5.0", AdvancedDisplay = "ForwardColor,RightColor,UpColor"))
		static void DrawDebugCoordinateArrowsAtComponent(USceneComponent* InComponent, FVector Offset, 
			float ArrowLength, float ArrowSize, float ArrowThickness, float DebugDisplayTime = 5.0f, 
			FLinearColor ForwardColor = FLinearColor::Red, FLinearColor RightColor = FLinearColor::Green, FLinearColor UpColor = FLinearColor::Blue);

		/**
		* Attempts to snap the inputted actor (using the pivot point) to the ground(which is really just a down direction)
		* @param TraceDistance The distance to trace for
		* @param GroundChannel The ground collision channel to trace against and will snap to
		* @param ActorsToIgnore Automatically adds the Spline Component's actor(ignores self)
		* @param OptionalOffset -Optional- Offsets the actor's snap to point on the ground by this value
		* @param bDrawDebug When true will show the trace using debug lines for DrawDebugTime's duration using the colors from TraceColor(Start to ImpactPoint or end if no hit) and TraceHitColor(ImpactPoint to End)
		* @return Returns true if successfully snapped actor to ground, false if actor is invalid or hit nothing to snap to.
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library",
			meta = (TraceDistance = "1000.0", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceColor,TraceHitColor,DrawDebugTime"))
			static bool SnapActorToGround(AActor* InActor, float TraceDistance, bool bTraceComplex,
				ETraceTypeQuery GroundChannel, const TArray<AActor*>& ActorsToIgnore, FVector OptionalOffset, bool bDrawDebug = false,
				FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawDebugTime = 5.0f);

#pragma region Spline Component Stuff

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static float SetSplineMeshRelativeRoll(USplineComponent* SplineComp, 
				const FRotator RelativeRotation, const float DistanceAlongSpline, const bool bReturnInRadians = false);

		/** Returns true if InPoint is a valid spline point. False if otherwise. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
		static bool IsValidSplinePoint(USplineComponent* SplineComp, int32 InPoint);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
		static int32 GetLastSplinePoint(USplineComponent* SplineComp);

		/** 
		* Attempts to snap all spline points in the inputted spline component to the ground(which is really just a down direction).
		* @param TraceDistance The distance to trace for
		* @param TraceChannel The collision channels to trace against and will snap to
		* @param ActorsToIgnore Automatically adds the Spline Component's actor(ignores self)
		* @param bDrawDebug When true will show the trace using debug lines for DrawDebugTime's duration using the colors from TraceColor(Start to ImpactPoint or end if no hit) and TraceHitColor(ImpactPoint to End)
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Spline", 
			meta = (TraceDistance = "1000.0", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceColor,TraceHitColor,DrawDebugTime"))
		static void SnapAllSplinePointsToGround(USplineComponent* SplineComp, float TraceDistance, bool bTraceComplex, 
			ETraceTypeQuery TraceChannel, const TArray<AActor*>& ActorsToIgnore, bool bDrawDebug = false,
			FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawDebugTime = 5.0f);

		/**
		* Attempts to snap a single spline point in the inputted spline component to the ground(which is really just a down direction).
		* @param TraceDistance The distance to trace for
		* @param TraceChannel The collision channels to trace against and will snap to
		* @param ActorsToIgnore Automatically adds the Spline Component's actor(ignores self)
		* @param bDrawDebug When true will show the trace using debug lines for DrawDebugTime's duration using the colors from TraceColor(Start to ImpactPoint or end if no hit) and TraceHitColor(ImpactPoint to End)
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Spline",
			meta = (TraceDistance = "1000.0", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceColor,TraceHitColor,DrawDebugTime"))
			static void SnapSingleSplinePointToGround(USplineComponent* SplineComp, int32 SplinePointToSnap, float TraceDistance, bool bTraceComplex,
				ETraceTypeQuery TraceChannel, const TArray<AActor*>& ActorsToIgnore, bool bDrawDebug = false,
				FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawDebugTime = 5.0f);

		/** Get location along spline at the provided input key value */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static FVector FindLocationAtSplineInputKey(USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace);

		/** Get tangent along spline at the provided input key value */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static FVector FindTangentAtSplineInputKey(USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace);

		/** Get unit direction along spline at the provided input key value */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static FVector FindDirectionAtSplineInputKey(USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace);

		/** Get rotator corresponding to rotation along spline at the provided input key value */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static FRotator FindRotationAtSplineInputKey(USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace);

		/** Get up vector at the provided input key value along spline */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static FVector FindUpVectorAtSplineInputKey(USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace);

		/** Get up vector at the provided input key value along spline */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static FVector FindRightVectorAtSplineInputKey(USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace);

		/** Get transform at the provided input key value along spline */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static FTransform FindTransformAtSplineInputKey(USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace, bool bUseScale = false);

		/** Get roll in degrees at the provided input key value along spline */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static float FindRollAtSplineInputKey(USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace);

		/** Get scale at the provided input key value along spline */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static FVector FindScaleAtSplineInputKey(USplineComponent* SplineComp, float InKey);

		/** Gets the location and rotation along spline, both at the provided input key value */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Spline")
			static void FindLocationAndRotationAtSplineInputKey(FVector& Location, FRotator& Rotation, USplineComponent* SplineComp, float InKey, ESplineCoordinateSpace::Type CoordinateSpace);

		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Spline")
			static TArray<USplineMeshComponent*> BuildSplineMeshesAlongSpline(
			USplineComponent* SplineComp, FExtraSplineConstructionInfo ConstructionInfo);

#pragma endregion

#pragma region Replay System Stuff

		/**
		* Start recording a replay with the given custom name and friendly name.
		*		
		* @param InName If not empty, the unique name to use as an identifier for the replay. If empty, a name will be automatically generated by the replay streamer implementation.
		* @param FriendlyName An optional (may be empty) descriptive name for the replay. Does not have to be unique.
		* @param AdditionalOptions Additional URL options to append to the URL that will be processed by the replay net driver. Will usually remain empty.
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
		static void StartRecordingReplay(const UObject* WorldContextObject, const FString& ReplayName,
			const FString& FriendlyName);

		/** Stop recording a replay if one is currently in progress */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static void StopRecordingReplay(const UObject* WorldContextObject);

		/** Returns true if a replay is being recorded or playing, false if otherwise. */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
		static bool IsReplayCurrentlyActive(const UObject* WorldContextObject);

		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
		static void SaveReplayCheckpoint(const UObject* WorldContextObject);

		/** 
		* Start playing back a previously recorded replay.
		* Return false if it fails to play.		
		*
		* @param InName If not empty, the unique name to use as an identifier for the replay. If empty, a name will be automatically generated by the replay streamer implementation.		
		* @param AdditionalOptions Additional URL options to append to the URL that will be processed by the replay net driver. Will usually remain empty.
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static bool PlayReplay(const UObject* WorldContextObject, const FString& ReplayName);

		/**
		* Adds a join-in-progress user to the set of users associated with the currently recording replay (if any)	
		*
		* @param UserString a string that uniquely identifies the user, usually the UniqueNetId
		*/
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static void AddUserToReplay(const UObject* WorldContextObject, const FString& UserString);

		/** Handles jumping the replay to the specified time */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static void JumpToTimeInReplay(float ReplayTime, const UObject* WorldContextObject);

		/** Returns if the replay is paused or not, will return false even if couldnt access replay system */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static bool IsReplayPaused(const UObject* WorldContextObject);

		/** Handles pausing and resuming the replay system */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static void ToggleReplayPauseState(const UObject* WorldContextObject);

		/** Handles setting the current replay to paused or not */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static void SetReplayPausedState(bool NewState, const UObject* WorldContextObject);

		/** Handles setting the replay playback speed in the world settings */
		UFUNCTION(BlueprintCallable, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static void SetReplayPlaybackSpeed(const UObject* WorldContextObject, float NewReplaySpeed = 1.0f);

		/** Gets the current recorded replay speed */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static float GetReplayPlaybackSpeed(const UObject* WorldContextObject);

		/** Gets the Total Number of Seconds that were recorded in the current Replay */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static float GetCurrentReplayTotalTimeInSeconds(const UObject* WorldContextObject);

		/** Gets the Second we are currently recording/watching in the Replay */
		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Replay", meta = (WorldContext = "WorldContextObject"))
			static float GetCurrentReplayCurrentTimeInSeconds(const UObject* WorldContextObject);

#pragma endregion

#pragma region Hit Result Breaking

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result", meta = (BlueprintAutocast))
			static bool GetHitBlockingHit(const FHitResult& Hit) { return Hit.bBlockingHit; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result")
			static bool GetHitInitialOverlap(const FHitResult& Hit) { return Hit.bStartPenetrating; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result", meta = (BlueprintAutocast))
			static float GetHitTime(const FHitResult& Hit) { return Hit.Time; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result")
			static float GetHitDistance(const FHitResult& Hit) { return Hit.Distance; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result")
			static FVector GetHitLocation(const FHitResult& Hit) { return Hit.Location; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result", meta = (BlueprintAutocast))
			static FVector GetHitImpactPoint(const FHitResult& Hit) { return Hit.ImpactPoint; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result")
			static FVector GetHitNormal(const FHitResult& Hit) { return Hit.Normal; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result")
			static FVector GetHitImpactNormal(const FHitResult& Hit) { return Hit.ImpactNormal; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result", meta = (BlueprintAutocast))
			static UPhysicalMaterial* GetHitPhysMat(const FHitResult& Hit);

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result", meta = (BlueprintAutocast))
			static AActor* GetHitActor(const FHitResult& Hit) { return Hit.GetActor(); }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result", meta = (BlueprintAutocast))
			static UPrimitiveComponent* GetHitComponent(const FHitResult& Hit) { return Hit.GetComponent(); }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result", meta = (BlueprintAutocast))
			static FName GetHitBoneName(const FHitResult& Hit) { return Hit.BoneName; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result")
			static int32 GetHitItem(const FHitResult& Hit) { return Hit.Item; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result", meta = (BlueprintAutocast))
			static int32 GetHitFaceIndex(const FHitResult& Hit) { return Hit.FaceIndex; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result")
			static FVector GetHitTraceStart(const FHitResult& Hit) { return Hit.TraceStart; }

		UFUNCTION(BlueprintPure, Category = "Extra Functionality Library|Hit Result")
			static FVector GetHitTraceEnd(const FHitResult& Hit) { return Hit.TraceEnd; }

#pragma endregion

};

