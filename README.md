# Unreal Engine Editor Blueprint Copilot Plugin

This project is a plugin for the Unreal Engine Editor.

The learning curve for developing in Unreal Engine can be quite steep, especially if you don't have an engineering background.

The aim of this plugin is to make Unreal Engine more accessible to a broader audience.

It allows you to tell OpenAI what you want to do and it performs it automatically for you.

## Using the Plugin

### Importing the plugin in your project

In order to import the plugin in your project:
- Create a `Plugins` folder at the root directory of your UE project
- Create a `BlueprintCopilot` folder inside the `Plugins` folder
- Place the content of this repository in `BlueprintCopilot/`
- Start your UE project
- In Edit -> Plugins -> Select BlueprintCopilot
- Close and open your project again for the plugin to be loaded (it might take a certain time because it needs to build it the first time)

If everything went well, you should see a `BlueprintCopilot` entry in the `Tools` toolbar, under the `Tools` section.

### Adding your API Key

In Edit -> Editor Preferences -> BlueprintCopilot:
Enter your OpenAPI key in the `API key` field. (TODO)

In order to get a key from OpenAPI, you need to create it [here](https://platform.openai.com/account/api-keys), and don't forget to add funds to your account.

If you're using ChatGPT and not OpenAI, no need to add an API key.
However, your first message will need to be the exact same as `Source/Deps/LibBlueprintCopilot/src/LibGuidance/include/Instructions.h` SystemData, and add your input below `[INSTRUCTIONS]`. In the plugin's text input, you can copy paste the response from ChatGPT.

### Making your first interaction
> [!WARNING]
> Be aware that this project is still in early development.
> **We strongly suggest that you save your modifications before using the plugin**
> **You've been warned**
> We cannot guarantee that what GPT instructs to do actually do exactly what you described.
> While we also aim for stability of the system, we do not guarantee that it will not crash Unreal.

Open the plugin, either via shortcut (Editor -> Editor Preferences -> Keyboard Shortcuts -> BlueprintCopilot) or `Tools -> Tools -> BlueprintCopilot`

Here you can select which API you want to use:
- OpenAI: this will make a call to OpenAI API, you can tell what you want to do in natural language (only English has been tested)
- Fake: this will reply with your input, this can be useful if you're using ChatGPT or debugging and don't want to use OpenAI credits
(TODO image)

> [!NOTE]
> When choosing OpenAI, it might take a while before getting an answer, all the time is spent waiting for the reply.

> [!IMPORTANT]
> We currently do not support modification of existing blueprints, or asking for further modification.

> [!IMPORTANT]
> There is currently no support for a dry run, or asking for confirmation before executing every command

#### Example

Here is an example that you can ask:
> Help me create a blueprint "TestBLUEPRINT" that has 3 int variables, a first one MyVariable that has value 5,
> a second one MyOtherVariable that has value 10, and a third one MyResult.
> Once created I want to addition them and store the result in MyResult

Here is an example of answer (which you can use in the Fake option)
```json
[
  {
    "Type": "CreateBlueprint",
    "BlueprintName": "TestBLUEPRINT",
    "BlueprintID": "ID_TestBLUEPRINT",
    "Reasoning": "Creating the blueprint named 'TestBLUEPRINT'."
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyVariable",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "5",
    "Reasoning": "Adding 'MyVariable' of type int with default value 5."
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyOtherVariable",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "10",
    "Reasoning": "Adding 'MyOtherVariable' of type int with default value 10."
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyResult",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "",
    "Reasoning": "Adding 'MyResult' variable to store the result of the addition."
  },
  {
    "Type": "AddVariableGetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyVariable",
    "NodeID": "ID_GetNode_MyVariable",
    "Reasoning": "Creating a getter node for 'MyVariable'."
  },
  {
    "Type": "AddVariableGetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyOtherVariable",
    "NodeID": "ID_GetNode_MyOtherVariable",
    "Reasoning": "Creating a getter node for 'MyOtherVariable'."
  },
  {
    "Type": "AddFunctionNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "FunctionName": "UKismetMathLibrary::Add_IntInt",
    "NodeID": "ID_AddFunctionNode",
    "Reasoning": "Creating a node for addition operation."
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_GetNode_MyVariable",
    "SourcePinName": "MyVariable",
    "DestinationNodeID": "ID_AddFunctionNode",
    "DestinationPinName": "A",
    "Reasoning": "Linking 'MyVariable' to the addition function input A."
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_GetNode_MyOtherVariable",
    "SourcePinName": "MyOtherVariable",
    "DestinationNodeID": "ID_AddFunctionNode",
    "DestinationPinName": "B",
    "Reasoning": "Linking 'MyOtherVariable' to the addition function input B."
  },
  {
    "Type": "AddVariableSetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyResult",
    "NodeID": "ID_SetNode_MyResult",
    "Reasoning": "Creating a setter node for 'MyResult'."
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_AddFunctionNode",
    "SourcePinName": "ReturnValue",
    "DestinationNodeID": "ID_SetNode_MyResult",
    "DestinationPinName": "MyResult",
    "Reasoning": "Linking the result of the addition to the 'MyResult' setter."
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_GetNode_MyVariable",
    "x": 100,
    "y": 100,
    "Reasoning": "Positioning the getter node for 'MyVariable'."
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_GetNode_MyOtherVariable",
    "x": 100,
    "y": 200,
    "Reasoning": "Positioning the getter node for 'MyOtherVariable'."
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_AddFunctionNode",
    "x": 250,
    "y": 150,
    "Reasoning": "Positioning the addition function node."
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_SetNode_MyResult",
    "x": 400,
    "y": 150,
    "Reasoning": "Positioning the setter node for 'MyResult'."
  },
  {
    "Type": "UpdateBlueprint",
    "BlueprintID": "ID_TestBLUEPRINT",
    "Reasoning": "Compiling and updating the blueprint after changes."
  },
  {
    "Type": "CreateBlueprintPermanently",
    "BlueprintID": "ID_TestBLUEPRINT",
    "Reasoning": "Saving the blueprint on disk to make it permanent."
  }
]
```

#### List of supported operations:
```json
CreateBlueprint(BlueprintName, BlueprintID) : Creates a blueprint, basically calling
	FKismetEditorUtilities::CreateBlueprint, is stored internally with BlueprintID and referenced with it for
	future actions.

AddVariable(BlueprintID, VariableName, PinCategory, DefaultValue) : Adds a member variable variableName
	to a blueprint, with a value if provided, be careful with the pin category, it must be of type
	UEdGraphSchema_K2, if you don't know what to put for DefaultValue, just put an empty string

AddVariableGetNode(BlueprintID, PropertyName, NodeID) : Creates a node NodeID in BlueprintID for the
	propertyName, this is a getter node

AddFunctionNode(BlueprintID, FunctionName, NodeID) : Creates a node NodeID in BlueprintID for the
	functionName, the functionName is for instance something inside UKismetMathLibrary

AddVariableSetNode(BlueprintID, PropertyName, NodeID) : Creates a node NodeID in BlueprintID for the
	propertyName, this node serves to set a value to the propertyName

CreateLink(SourceNodeID, SourcePinName, DestinationNodeID, DestinationPinName) : Links
	SourceNodeID->SourcePinName to DestinationNodeID->DestinationPinName via FindPin and MakeLinkTo

PositionNode(NodeID, x, y) : Position NodeID at x, y coordinate in the event graph, Make sure to leave
	some space for the default created Nodes, be careful, when outputing the json, x and y must be integers,
	not strings

UpdateBlueprint(BlueprintID) : Marks the blueprint as modified, and compile it

CreateBlueprintPermanently(BlueprintID) : Creates the blueprint on disk

ManualOperation(WhatToDo) : Some manual steps that your human companion will have to do
```

If you're interested to see what kind of commands is supported, you can take a look at `Source/Deps/LibBlueprintCopilot/src/LibGuidance/include/Instructions.h`


## Developing

- Create a Unreal Engine 5.2 project
- Inside the project, create a `Plugins` folder
- Move this folder inside `Plugins` folder
- In the editor, add the plugin to the project, you should see it by filtering local plugins or by name

If you create a C++ project, you'll be able to set breakpoints and debug, otherwise you'll have to do the following steps for changes to be reflected:
- Remove the plugin's `Binaries` and `Intermediate` folders
- Start the project again

> [!WARNING]
> If you've made changes in the plugin and when restarting the project it doesn't ask you to re-compile the plugins, it will not take your changes into account.

TODO: A nicer way to debug than printing UE_LOG would be highly appreciated.

And looking at the project's `Saved/Logs/<project_name>.log` and add logging statement in the plugin to see where the problems occurs.

The first build can take quite some time, as it builds dependencies like Curl.

### Project structure

The project is structured in 4 parts:
- A CMake library LibGuidance (Source/Deps/LibBlueprintCopilot/src/LibGuidance)
- A CMake test application (Source/Deps/LibBlueprintCopilot/src/TestApplication)
- The Editor's plugin
- [UE4CMake](https://github.com/caseymcc/UE4CMake/tree/main), which allows the linking of CMake libraries into the plugin

#### LibGuidance

This library makes the interface with the llm you're interacting with.
Currently it supports two modes:
- Fake: Only responds with your input, convenient if you're debugging parts of the system based on an previous reply or ChatGPT in a faster, deterministic way and without using credits.
- OpenAI: Calls the OpenAI API, you need to have an [API key](https://platform.openai.com/account/api-keys) and some credits for it to work. We're using the unofficial [liboai](https://github.com/D7EAD/liboai) project to call it in a synchronous way, it is not very fast though.

The system data is defined in `include/Instructions.h`

We also take a dependency on [nlohmann_json](https://github.com/nlohmann/json) for the serialization and deserialization of the commands, this is done conveniently by using `NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE`

This library does not take any dependeny on UE and will not.

#### Adding a new dependency

If you're adding a new dependency, in CMake it's quite simple, but you must also do the following for the plugin to continue working:
- Add the dependency to BlueprintCopilot.Build.cs (see how it's done for our lib, curl and oai).
- If it adds a runtime dependency (like curl, you'll see quickly that the UE project can't be loaded anymore and in the logs it will complain that it was not able to import your dll),
  you'll need to copy the dll into `Binaries/Win64/` of the UE project.
  This can be achieved in the `PostBuildSteps` of `BlueprintCopilot.uplugin`, see the example for `libcurl.dll`

##### How to use

```cpp
	// Create the interface
    auto libLLM{LibLLMFactory::CreateLibLLM(LibLLMModel::(OpenAILLMModel or FakeLLMModel), "<your api key>")};

	// Send the input, this is a synchronous call, it will block and take quite some time if you use OpenAI API
	auto response{libLLM->Request("Your input")};

	// Get the list of actions to execute
    auto actions{ConvertResponseToActions(response)};
```

##### Adding a new command

If you find yourself wanting to add a new command, you'll have to follow the following steps:
- Create a new command in `include/Actions/`
- Add the file to the `CMakeLists.txt` for LibGuidance library
- Add it to the `Action` variant: `include/Actions/Action.h`
- Add a deserialization method in `src/ActionsDeserialization.cpp`
- Add the command in `include/Instructions.h` with a comment for the llm to know what to do with it

Important: Because UE works with pointers for all its objects, when we create blueprints and nodes, we need to keep track of what node the command needs to target or create,
that is the reason you see all commands taking BlueprintID or NodeID, it's to ensure the instructions can target the right nodes.

You'll also need to add a function to the UE4 plugin to transform from the command to operation inside UE (see Plugin's section below).

###### Manual Operations

There is a special command ManualOperation, this basically means that in the set of actions we propose, there is nothing we offer it to perform an action.
It is a good indication that we need to add a new command to help it perform the operations programatically.

#### Test Application

This project is a console app that serves the purpose of testing LibGuidance.
It has nothing to do with Unreal Engine, but serves the purpose of providing a fast way to iterate and debug on all the operations that happen before UE.

#### UE4CMake

This is the part that allows to link our CMake project to our plugin, hence making LibGuidance available to it.
It is taken straight from [UE4CMake](https://github.com/caseymcc/UE4CMake/tree/main) with minor modification to make it work in our context.

#### BlueprintCopilot (Plugin)

##### Plugin structure

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

##### UI and Logic separation
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

##### Adding a command

If you're adding a command, make sure to also add the part that will be able to translate it to Unreal Engine code.

It must follow the same pattern as others:
```cpp
bool PerformAction(const LibBlueprintCopilot::Guidance::YourNewAction& action)
{
    const auto success{YourNewAction(action.field, action.field2, ...)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to do some stuff"));
        return success;
    }

    return success;	
}
```

You might notice that some actions take an ID (Blueprint, Node), it's to ensure that the action is performed on the intended node.

We internally cache the pointers to the created Blueprint and Nodes, so you can ask the cache for the object with the ID key.

For instance, if your new action is creating a node, make sure that you store it in the internal cache.

TODO more stuff