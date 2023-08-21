# Unreal Engine Editor Plugin Template
![a1](https://user-images.githubusercontent.com/7863125/197690874-50dc0777-35df-47a1-9f24-29af3b7e0743.png)
![a2](https://user-images.githubusercontent.com/7863125/197690879-788a860f-2ae6-4439-b6ff-4f8c9e971f43.png)

This is a template of a plugin for UE4 that contains an Editor Utility Widget and can be opened via drop-down menu or a keyboard shortcut.

## Plugin structure

The plugin template is made of the following classes:

* **FBlueprintCopilotModule**
Contains a logic that registers every needed component when started. It has a function for invoking the spawn of the editor's window and it constructs the plugin's logic object.
* **UBlueprintCopilotBase**
The base of the object that contains the plugin's logic. This base handles only the widget creation and travels between levels. Any additional logic is handled by it's extension.
* **UBlueprintCopilot**
The extension of the base object. **Any logic that the plugin does should ends up here**.
* **UBlueprintCopilotWidget**
The base class of the widget which is stored inside the plugin's content directory and which is displayed in the editors window.
* **FBlueprintCopilotStyle**
Plugin style handle. Used to set the icon inside the Editor's UI.
* **FBlueprintCopilotCommands**
Commands handle. Used to register a keyboard shortcut that opens the plugin's window.

## Changing the plugin widget
Inside the plugin's content directory there is a `BlueprintCopilotWidget_BP` Editor Utility Widget which is opened when the plugin is invoked. The base class of this widget is `UBlueprintCopilotWidget`. When the name of the widget is changed the following path in the `UBlueprintCopilotBase` must bechanged:

``` cpp
#if ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 1))
	FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FSoftObjectPath("/BlueprintCopilot/BlueprintCopilotWidget_BP.BlueprintCopilotWidget_BP"));
#else
	FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath("/BlueprintCopilot/BlueprintCopilotWidget_BP.BlueprintCopilotWidget_BP");
#endif
```

## UI and Logic separation
This plugin template separates the UI and the Logic layers. All the UI part is inside the `UBlueprintCopilotWidget`. All the logic part is inside the `UBlueprintCopilot` object. Communication between these elements is handled by the following scheme:
 * UI -> Logic : by delegates
 ``` cpp
// BlueprintCopilot.cpp
void UBlueprintCopilot::InitializeTheWidget()
{
    EditorWidget->OnTestButtonPressedDelegate.BindUObject(this, &UBlueprintCopilot::OnTestButtonPressed);
}

// BlueprintCopilotWidget.cpp
void UBlueprintCopilotWidget::TestButtonPressed()
{
	OnTestButtonPressedDelegate.ExecuteIfBound();
}
```
 * Logic -> UI : by direct function call
``` cpp
// BlueprintCopilot.cpp
void UBlueprintCopilot::OnTestButtonPressed()
{
	EditorWidget->SetNumberOfTestButtonPressed(NumberOfTestButtonPressed);
}
```

## Developing

- Create a Unreal Engine 5.2 project
- Inside the project, create a `Plugins` folder
- Move this folder inside `Plugins` folder
- In the editor, add the plugin to the project, you should see it by filtering local plugins or by name

If you create a C++ project, you'll be able to set breakpoints and debug, otherwise you'll have to do the following steps for changes to be reflected:
- Remove the plugin's `Binaries` and `Intermediate` folders
- Start the project again

And looking at the project's `Saved/Logs/<project_name>.log` and add logging statement in the plugin to see where the problems occurs
