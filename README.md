# ExtraFunctionality
Nick's dope extra functionality library for UE4 stuff

## How do I use?! (Setup Guide)

1. Make sure you have a C++ class in your project(and of course the correct version of visual studio installed). A way to check is if you have a Source folder in your project([ProjectName]>Source, and not anywhere in the content folder), that means you have a C++ class(an empty one is fine). 
   - If you don't have a C++ class then you can create one in the editor by going to File>Create C++ Class>Empty Class, then create class(it doesn't matter mostly since your not gonna touch this but it just needs to exist). Unreal Engine 4 requires all project based C++ plugins to have at least 1 C++ class in the project in general.

2. Create a Plugins folder in your project's folder(not anywhere in the content folder) for example: [ProjectName]>Plugins. Close the Unreal Engine project if its currently open.

3. Copy(or have github download the plugin) into the Plugins folder you made.

4. On your uproject file, right click on it and select **Generate Visual Studio project files**. It should succeed with no problems(if it does, please let me know so I can help you (: ). But doing this will create the binaries and intermediate files for the plugin and add it to the visual studio project.

5. Open the sln file that should be directly above your uproject file(Example Name: [ProjectName].sln) as this is your visual studio(C++) project of the unreal engine project.
   - If you already have visual studio open, it will probably ask about reloading or something similar simply select reload or Overwrite all and then give a few seconds to update the project to use the new files added from the plugin.

6. After its fully opened(which may take a while depending on your machine), at the top select Build>Build [ProjectName] and then wait. Visual studio is building the C++ code of your project and reloading the Unreal Engine project files to use these newly cooked C++ files/changes. You will be able to tell if its finished building if the output log says something along the lines of "Succeeded: 2 Failed: 0" what we want is everything to succeed and have no failures.

7. Thats it! The plugin should be integrated into the project and when you open the unreal engine project by double clicking on the uproject file. Also make sure the plugin is enabled (or at least visible) in the plugins window after the project opens but it may require a restart of the project just to turn it on if it isn't.

- [Back To Top](https://github.com/Oldsiren/ExtraFunctionality#ExtraFunctionality)
