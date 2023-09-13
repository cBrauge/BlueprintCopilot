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
        "content": "[\n    {\n        \"Type\": \"CreateBlueprint\",\n        \"BlueprintName\": \"TestBLUEPRINT\",\n        \"BlueprintID\": \"test_blueprint\"\n    },\n    {\n        \"Type\": \"AddVariable\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"VariableName\": \"MyVariable\",\n        \"PinCategory\": \"UEdGraphSchema_K2::PC_Int\",\n        \"DefaultValue\": \"5\"},\n    {\n        \"Type\": \"AddVariable\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"VariableName\": \"MyOtherVariable\",\n        \"PinCategory\": \"UEdGraphSchema_K2::PC_Int\",\n        \"DefaultValue\": \"10\",\n        \"Reasoning\": \"Adding variable MyOtherVariable with default value 10 and type UEdGraphSchema_K2::PC_Int to TestBLUEPRINT\"\n    },\n    {\n        \"Type\": \"AddVariable\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"VariableName\": \"MyResult\",\n        \"PinCategory\": \"UEdGraphSchema_K2::PC_Int\",\n        \"DefaultValue\": \"\",\n        \"Reasoning\": \"Adding variable MyResult with default value '' and type UEdGraphSchema_K2::PC_Int to TestBLUEPRINT\"\n    },\n    {\n        \"Type\": \"AddVariableGetNode\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"PropertyName\": \"MyVariable\",\n        \"NodeID\": \"get_node_1\"\n    },\n    {\n        \"Type\": \"AddVariableGetNode\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"PropertyName\": \"MyOtherVariable\",\n        \"NodeID\": \"get_node_2\"\n    },\n    {\n        \"Type\": \"AddFunctionNode\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"FunctionName\": \"Add_IntInt\",\n        \"NodeID\": \"add_node\"\n    },\n    {\n        \"Type\": \"PositionNode\",\n        \"NodeID\": \"get_node_1\",\n        \"x\": 0,\n        \"y\": 0\n    },\n    {\n        \"Type\": \"PositionNode\",\n        \"NodeID\": \"get_node_2\",\n        \"x\": 0,\n        \"y\": 200\n    },\n    {\n        \"Type\": \"PositionNode\",\n        \"NodeID\": \"add_node\",\n        \"x\": 400,\n        \"y\": 100\n    },\n    {\n        \"Type\": \"CreateLink\",\n        \"SourceNodeID\": \"get_node_1\",\n        \"SourcePinName\": \"ReturnValue\",\n        \"DestinationNodeID\": \"add_node\",\n        \"DestinationPinName\": \"A\"\n    },\n    {\n        \"Type\": \"CreateLink\",\n        \"SourceNodeID\": \"get_node_2\",\n        \"SourcePinName\": \"ReturnValue\",\n        \"DestinationNodeID\": \"add_node\",\n        \"DestinationPinName\": \"B\"\n    },\n    {\n        \"Type\": \"AddVariableSetNode\",\n        \"BlueprintID\": \"test_blueprint\",\n        \"PropertyName\": \"MyResult\",\n        \"NodeID\": \"set_node\"\n    },\n    {\n        \"Type\": \"PositionNode\",\n        \"NodeID\": \"set_node\",\n        \"x\": 800,\n        \"y\": 100\n    },\n    {\n        \"Type\": \"CreateLink\",\n        \"SourceNodeID\": \"add_node\",\n        \"SourcePinName\": \"Result\",\n        \"DestinationNodeID\": \"set_node\",\n        \"DestinationPinName\": \"NewValue\"\n    },\n    {\n        \"Type\": \"UpdateBlueprint\",\n        \"BlueprintID\": \"test_blueprint\"\n    },\n    {\n        \"Type\": \"CreateBlueprintPermanently\",\n        \"BlueprintID\": \"test_blueprint\"\n    }\n]"
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
    "BlueprintType": "Actor"
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyVariable",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "5"
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyOtherVariable",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "10"
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyResult",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": ""
  },
 {
    "Type": "UpdateBlueprint",
    "BlueprintID": "ID_TestBLUEPRINT"
  },
  {
    "Type": "AddVariableGetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyVariable",
    "NodeID": "ID_GetNode_MyVariable"
  },
  {
    "Type": "AddVariableGetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyOtherVariable",
    "NodeID": "ID_GetNode_MyOtherVariable"
  },
  {
    "Type": "AddFunctionNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "FunctionName": "KismetMathLibrary::Add_IntInt",
    "NodeID": "ID_AddFunctionNode"
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_GetNode_MyVariable",
    "SourcePinName": "MyVariable",
    "DestinationNodeID": "ID_AddFunctionNode",
    "DestinationPinName": "A"
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_GetNode_MyOtherVariable",
    "SourcePinName": "MyOtherVariable",
    "DestinationNodeID": "ID_AddFunctionNode",
    "DestinationPinName": "B"
  },
  {
    "Type": "AddVariableSetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyResult",
    "NodeID": "ID_SetNode_MyResult"
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_AddFunctionNode",
    "SourcePinName": "ReturnValue",
    "DestinationNodeID": "ID_SetNode_MyResult",
    "DestinationPinName": "MyResult"
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_GetNode_MyVariable",
    "x": 100,
    "y": 100
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_GetNode_MyOtherVariable",
    "x": 100,
    "y": 200
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_AddFunctionNode",
    "x": 250,
    "y": 150
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_SetNode_MyResult",
    "x": 400,
    "y": 150
  },
  {
    "Type": "UpdateBlueprint",
    "BlueprintID": "ID_TestBLUEPRINT"
  },
  {
    "Type": "CreateBlueprintPermanently",
    "BlueprintID": "ID_TestBLUEPRINT"
  }
]
    )"};

    const auto ListOfInstructionsWithNotJustJsonArray{R"(
Some leading stuff
[
  {
    "Type": "CreateBlueprint",
    "BlueprintName": "TestBLUEPRINT",
    "BlueprintID": "ID_TestBLUEPRINT"
  }
]
Some stuff at the end
)"};

    const auto ListOfInstructionsNotNumber{R"([
  { "Type": "CreateBlueprint", "BlueprintName": "TestBLUEPRINT", "BlueprintID": "TestBLUEPRINT_ID", "BlueprintType": "Actor"},
  { "Type": "AddVariable", "BlueprintID": "TestBLUEPRINT_ID", "VariableName": "MyVariable", "PinCategory": "UEdGraphSchema_K2::PC_Int", "DefaultValue": "5"},
  { "Type": "AddVariable", "BlueprintID": "TestBLUEPRINT_ID", "VariableName": "MyOtherVariable", "PinCategory": "UEdGraphSchema_K2::PC_Int", "DefaultValue": "10"},
  { "Type": "AddVariable", "BlueprintID": "TestBLUEPRINT_ID", "VariableName": "MyResult", "PinCategory": "UEdGraphSchema_K2::PC_Int", "DefaultValue": ""},
  { "Type": "AddVariableGetNode", "BlueprintID": "TestBLUEPRINT_ID", "PropertyName": "MyVariable", "NodeID": "MyVariableGetNode" },
  { "Type": "AddVariableGetNode", "BlueprintID": "TestBLUEPRINT_ID", "PropertyName": "MyOtherVariable", "NodeID": "MyOtherVariableGetNode" },
  { "Type": "AddFunctionNode", "BlueprintID": "TestBLUEPRINT_ID", "FunctionName": "KismetMathLibrary::Add_IntInt", "NodeID": "AddFunctionNode"},
  { "Type": "AddVariableSetNode", "BlueprintID": "TestBLUEPRINT_ID", "PropertyName": "MyResult", "NodeID": "MyResultSetNode"},
  { "Type": "CreateLink", "SourceNodeID": "MyVariableGetNode", "SourcePinName": "MyVariable", "DestinationNodeID": "AddFunctionNode", "DestinationPinName": "A" },
  { "Type": "CreateLink", "SourceNodeID": "MyOtherVariableGetNode", "SourcePinName": "MyOtherVariable", "DestinationNodeID": "AddFunctionNode", "DestinationPinName": "B" },
  { "Type": "CreateLink", "SourceNodeID": "AddFunctionNode", "SourcePinName": "ReturnValue", "DestinationNodeID": "MyResultSetNode", "DestinationPinName": "MyResult" },
  { "Type": "PositionNode", "NodeID": "MyVariableGetNode", "x": "100", "y": "200" },
  { "Type": "PositionNode", "NodeID": "MyOtherVariableGetNode", "x": "200", "y": "300" },
  { "Type": "PositionNode", "NodeID": "AddFunctionNode", "x": "400", "y": "300" },
  { "Type": "PositionNode", "NodeID": "MyResultSetNode", "x": "600", "y": "400" },
  { "Type": "UpdateBlueprint", "BlueprintID": "TestBLUEPRINT_ID"},
  { "Type": "CreateBlueprintPermanently", "BlueprintID": "TestBLUEPRINT_ID"}
])"};

    const auto ListOfInstructionsIncrementUI{
        R"(
[
    { "Type": "CreateBlueprint", "BlueprintName" : "CounterUI", "BlueprintID" : "bp_counterUI_id", "BlueprintType": "Widget"},
    { "Type": "AddVariable", "BlueprintID" : "bp_counterUI_id", "VariableName" : "Counter", "PinCategory" : "UEdGraphSchema_K2::PC_Int", "DefaultValue" : "0"},
    { "Type": "AssignNode", "BlueprintID" : "bp_counterUI_id", "NodeID": "event_construct_id", "NodeName": "Event Construct"},
    { "Type": "AddFunctionNode", "BlueprintID" : "bp_counterUI_id", "FunctionName" : "UKismetSystemLibrary::SetTimerByFunctionName", "NodeID" : "set_timer_id"},
    { "Type": "SetPinValue", "NodeID": "set_timer_id", "PinName": "FunctionName", "Value": "IncrementCounter"},
    { "Type": "SetPinValue", "NodeID": "set_timer_id", "PinName": "Time", "Value": "1.0"},
    { "Type": "SetPinValue", "NodeID": "set_timer_id", "PinName": "Looping", "Value": "true"},
    { "Type": "CreateLink", "SourceNodeID": "event_construct_id", "SourcePinName": "Exec", "DestinationNodeID": "set_timer_id", "DestinationPinName": "Exec"},
    { "Type": "AddFunctionNode", "BlueprintID" : "bp_counterUI_id", "FunctionName" : "UKismetMathLibrary::Add_IntInt", "NodeID" : "add_int_id"},
    { "Type": "AddVariableGetNode", "BlueprintID" : "bp_counterUI_id", "PropertyName" : "Counter", "NodeID" : "get_counter_id"},
    { "Type": "CreateLink", "SourceNodeID": "get_counter_id", "SourcePinName": "Counter", "DestinationNodeID": "add_int_id", "DestinationPinName": "A"},
    { "Type": "SetPinValue", "NodeID": "add_int_id", "PinName": "B", "Value": "1"},
    { "Type": "AddVariableSetNode", "BlueprintID" : "bp_counterUI_id", "PropertyName" : "Counter", "NodeID" : "set_counter_id"},
    { "Type": "CreateLink", "SourceNodeID": "add_int_id", "SourcePinName": "ReturnValue", "DestinationNodeID": "set_counter_id", "DestinationPinName": "Counter"},
    { "Type": "PositionNode", "NodeID": "event_construct_id", "x": 100, "y": 100},
    { "Type": "PositionNode", "NodeID": "set_timer_id", "x": 250, "y": 100},
    { "Type": "PositionNode", "NodeID": "get_counter_id", "x": 100, "y": 250},
    { "Type": "PositionNode", "NodeID": "add_int_id", "x": 250, "y": 250},
    { "Type": "PositionNode", "NodeID": "set_counter_id", "x": 400, "y": 250},
    { "Type": "UpdateBlueprint", "BlueprintID" : "bp_counterUI_id"},
    { "Type": "CreateBlueprintPermanently", "BlueprintID" : "bp_counterUI_id"}
]
)"};

    const auto ListOfInstructionsNoMathNamespace{
        R"([
  {
    "Type": "CreateBlueprint",
    "BlueprintName": "value sum2",
    "BlueprintID": "value_sum2",
    "BlueprintType": "Actor"
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "value_sum2",
    "VariableName": "num1",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "4"
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "value_sum2",
    "VariableName": "num2",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "3"
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "value_sum2",
    "VariableName": "sum",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": ""
  },
  {
    "Type": "AddVariableGetNode",
    "BlueprintID": "value_sum2",
    "PropertyName": "num1",
    "NodeID": "num1_get"
  },
  {
    "Type": "AddVariableGetNode",
    "BlueprintID": "value_sum2",
    "PropertyName": "num2",
    "NodeID": "num2_get"
  },
  {
    "Type": "AddFunctionNode",
    "BlueprintID": "value_sum2",
    "FunctionName": "Add_IntInt",
    "NodeID": "add_node"
  },
  {
    "Type": "AddVariableSetNode",
    "BlueprintID": "value_sum2",
    "PropertyName": "sum",
    "NodeID": "sum_set"
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "num1_get",
    "SourcePinName": "ReturnValue",
    "DestinationNodeID": "add_node",
    "DestinationPinName": "A"
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "num2_get",
    "SourcePinName": "ReturnValue",
    "DestinationNodeID": "add_node",
    "DestinationPinName": "B"
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "add_node",
    "SourcePinName": "Result",
    "DestinationNodeID": "sum_set",
    "DestinationPinName": "NewValue"
  },
  {
    "Type": "UpdateBlueprint",
    "BlueprintID": "value_sum2"
  },
  {
    "Type": "CreateBlueprintPermanently",
    "BlueprintID": "value_sum2"
  }
])"};
} // namespace LibBlueprintCopilot::Guidance