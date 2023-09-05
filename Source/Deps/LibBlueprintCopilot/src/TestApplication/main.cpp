#include "ActionsDeserialization.h"
#include "ChatGPT.h"
#include "LibBlueprintCopilot.h"
#include "LibLLMFactory.h"

#include <iostream>

// You're an expert in Unreal Engine. Based on the instructions written in natural language given by
// your human companion, which will be specified under the heading
//[INSTRUCTIONS], you will specify a series of actions that will
// needed to be performed to accomplish the task you are given as a goal.
// The things that you direct are listed below in the format �action(parameter1, parameter2, �,
//     paremeterN)� and a brief explanation is provided.
//     There are also special parameters BlueprintID and NodeID, they are there to allow caching internally, so make
//     sure you choose an ID that cannot clash with something else, you must provide them and make sure that you don't
//     reference one before it is actually created. Don't forget to position the nodes so that the companion can clearly
//     read the blueprint when opening it. Don't also forget to finish by CreateBlueprintPermanently If you add
//     variables with AddVariables, don't forget to call UpdateBlueprint before trying to do other actions like creating
//     the getter node for this variable
//
//     + CreateBlueprint(BlueprintName, BlueprintID) : Creates a blueprint, basically calling
//     FKismetEditorUtilities::CreateBlueprint, is stored internally with BlueprintID and referenced with it for future
//     actions.
//     + AddVariable(BlueprintID, VariableName, PinCategory, DefaultValue) : Adds a member variable variableName to a
//     blueprint, with a value if provided, be careful with the pin category, it must be of type UEdGraphSchema_K2, if
//     you don't know what to put for DefaultValue, just put an empty string
//     + AddVariableGetNode(BlueprintID, PropertyName, NodeID) : Creates a node NodeID in BlueprintID for the
//     propertyName, this is a getter node
//     + AddFunctionNode(BlueprintID, FunctionName, NodeID) : Creates a node NodeID in BlueprintID for the functionName,
//     the functionName is for instance something inside UKismetMathLibrary
//     + AddVariableSetNode(BlueprintID, PropertyName, NodeID) : Creates a node NodeID in BlueprintID for the
//     propertyName, this node serves to set a value to the propertyName
//     + CreateLink(SourceNodeID, SourcePinName, DestinationNodeID, DestinationPinName) : Links
//     SourceNodeID->SourcePinName to DestinationNodeID->DestinationPinName via FindPin and MakeLinkTo
//     + PositionNode(NodeID, x, y) : Position NodeID at x, y coordinate in the event graph, Make sure to leave some
//     space for the default created Nodes, be careful, when outputing the json, x and y must be integers, not strings
//     + UpdateBlueprint(BlueprintID) : Marks the blueprint as modified, and compile it
//     + CreateBlueprintPermanently(BlueprintID) : Creates the blueprint on disk
//     + ManualOperation(WhatToDo) : Some manual steps that your human companion will have to do
//
//     Your response will be a list of actions from the above list, with the
//     parameters filled in, and returned as a JSON - compatible array of
//     objects in the format : { �Type�: �Name of the action in the list�,
//     "Name of the parameter" : "Value for the parameter", �Reasoning� : �[an
//     explanation of why this step was chosen]�} Respond with nothing else,
//     just the JSON output, in a code block.
//     So for instance, if I needed to add an int variable "foo" to "name_of_my_blueprint_guiwbeibweifs1651" with
//     default value 5, it would look like this:
//{ "Type": "AddVariable", "BlueprintID" : "name_of_my_blueprint_guiwbeibweifs1651", "VariableName" : "foo",
//"PinCategory" : "UEdGraphSchema_K2::PC_Int", "DefaultValue" : "5", "Reason" : "Adding variable foo with default value
// 5 and type UEdGraphSchema_K2::PC_Int to name_of_my_blueprint"}
//
//[INSTRUCTIONS]
// Help me create a blueprint "TestBLUEPRINT" that has 3 int variables, a first one MyVariable that has value 5,
// a second one MyOtherVariable that has value 10, and a third one MyResult.
// Once created I want to addition them and store the result in MyResult

int main()
{
    auto libLLM{LibBlueprintCopilot::Guidance::LibLLMFactory::CreateLibLLM(
        LibBlueprintCopilot::Guidance::LibLLMModel::FakeLLMModel, "")};

    std::string jsonString = R"(
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

    )";

    auto response{libLLM->Request(jsonString)};
    auto actions{LibBlueprintCopilot::Guidance::ConvertResponseToActions(response)};
}