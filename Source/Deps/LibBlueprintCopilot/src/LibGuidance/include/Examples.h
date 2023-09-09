#pragma once

#include <string>

namespace LibBlueprintCopilot::Guidance
{
    // Example of the input from a user
    const auto Request{
        R"(Help me create a blueprint "TestBLUEPRINT" that has 3 int variables, a first one MyVariable that has value 5,
            a second one MyOtherVariable that has value 10, and a third one MyResult.
            Once created I want to addition them and store the result in MyResult)"};

    // Entire response from OpenAI API (if we were not using liboai)
    const auto RealResponseFromAPI{
        R"({
  "id": "chatcmpl-7vmo8c7NxUAJUCfQYf6fjltceFbRI",
  "object": "chat.completion",
  "created": 1694006668,
  "model": "gpt-3.5-turbo-0613",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "[\n    {\n        \"Type\": \"CreateBlueprint\",\n        \"BlueprintName\": \"TestBLUEPRINT\",\n        \"BlueprintID\": \"test_blueprint\"\n    },\n    {\n        \"Type\": \"AddVariable\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"VariableName\": \"MyVariable\",\n        \"PinCategory\": \"UEdGraphSchema_K2::PC_Int\",\n        \"DefaultValue\": \"5\",\n        \"Reasoning\": \"Adding variable MyVariable with default value 5 and type UEdGraphSchema_K2::PC_Int to TestBLUEPRINT\"\n    },\n    {\n        \"Type\": \"AddVariable\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"VariableName\": \"MyOtherVariable\",\n        \"PinCategory\": \"UEdGraphSchema_K2::PC_Int\",\n        \"DefaultValue\": \"10\",\n        \"Reasoning\": \"Adding variable MyOtherVariable with default value 10 and type UEdGraphSchema_K2::PC_Int to TestBLUEPRINT\"\n    },\n    {\n        \"Type\": \"AddVariable\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"VariableName\": \"MyResult\",\n        \"PinCategory\": \"UEdGraphSchema_K2::PC_Int\",\n        \"DefaultValue\": \"\",\n        \"Reasoning\": \"Adding variable MyResult with default value '' and type UEdGraphSchema_K2::PC_Int to TestBLUEPRINT\"\n    },\n    {\n        \"Type\": \"AddVariableGetNode\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"PropertyName\": \"MyVariable\",\n        \"NodeID\": \"get_node_1\"\n    },\n    {\n        \"Type\": \"AddVariableGetNode\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"PropertyName\": \"MyOtherVariable\",\n        \"NodeID\": \"get_node_2\"\n    },\n    {\n        \"Type\": \"AddFunctionNode\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"FunctionName\": \"Add_IntInt\",\n        \"NodeID\": \"add_node\"\n    },\n    {\n        \"Type\": \"PositionNode\",\n        \"NodeID\": \"get_node_1\",\n        \"x\": 0,\n        \"y\": 0\n    },\n    {\n        \"Type\": \"PositionNode\",\n        \"NodeID\": \"get_node_2\",\n        \"x\": 0,\n        \"y\": 200\n    },\n    {\n        \"Type\": \"PositionNode\",\n        \"NodeID\": \"add_node\",\n        \"x\": 400,\n        \"y\": 100\n    },\n    {\n        \"Type\": \"CreateLink\",\n        \"SourceNodeID\": \"get_node_1\",\n        \"SourcePinName\": \"ReturnValue\",\n        \"DestinationNodeID\": \"add_node\",\n        \"DestinationPinName\": \"A\"\n    },\n    {\n        \"Type\": \"CreateLink\",\n        \"SourceNodeID\": \"get_node_2\",\n        \"SourcePinName\": \"ReturnValue\",\n        \"DestinationNodeID\": \"add_node\",\n        \"DestinationPinName\": \"B\"\n    },\n    {\n        \"Type\": \"AddVariableSetNode\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"PropertyName\": \"MyResult\",\n        \"NodeID\": \"set_node\"\n    },\n    {\n        \"Type\": \"PositionNode\",\n        \"NodeID\": \"set_node\",\n        \"x\": 800,\n        \"y\": 100\n    },\n    {\n        \"Type\": \"CreateLink\",\n        \"SourceNodeID\": \"add_node\",\n        \"SourcePinName\": \"Result\",\n        \"DestinationNodeID\": \"set_node\",\n        \"DestinationPinName\": \"NewValue\"\n    },\n    {\n        \"Type\": \"UpdateBlueprint\",\n        \"BlueprintID\": \"test_blueprint\"\n    },\n    {\n        \"Type\": \"CreateBlueprintPermanently\",\n        \"BlueprintID\": \"test_blueprint\"\n    }\n]"
      },
      "finish_reason": "stop"
    }
  ],
  "usage": {
    "prompt_tokens": 933,
    "completion_tokens": 794,
    "total_tokens": 1727
  }
}
)"};

    // An example of response from OpenAI
    const auto ListOfInstructions{R"(
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
    "Type": "UpdateBlueprint",
    "BlueprintID": "ID_TestBLUEPRINT",
    "Reasoning": "Compiling and updating the blueprint after changes."
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
    "FunctionName": "Add_IntInt",
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
    )"};
} // namespace LibBlueprintCopilot::Guidance