// Copyright (c) 2022 Ren Brauge. All rights reserved.
#pragma once

/// @brief This functions shows what you'd have to do to perform the example request manually.
/// There are no references to our library
void ManualActionsToAddTwoVariables()
{
    // 1. Create a new Blueprint
    const auto blueprintName{"TestBLUEPRINT"};
    const auto blueprintID{"TestBLUEPRINT0"};
    const auto myVarName{"MyVariable"};
    const auto myVarNodeID{"MyVariableNode0"};
    const auto myOtherVarName{"MyOtherVariable"};
    const auto myOtherVarNodeID{"MyOtherVariableNode0"};
    const auto myResultName{"MyResult"};
    const auto myResultNodeID{"MyResultNode0"};
    const auto functionName{"Add_IntInt"};
    const auto functionNodeID{"Add_IntIntNode0"};

    auto NewBlueprint{CreateBlueprint(blueprintName, blueprintID)};
    if (!NewBlueprint)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create blueprint!"));
        return;
    }

    // 2. Create Variables
    auto MyVarSuccess{AddVariable(blueprintID, myVarName, UEdGraphSchema_K2::PC_Int, "5")};
    if (!MyVarSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add MyVar to blueprint!"));
        return;
    }

    auto MyOtherVarSuccess{AddVariable(blueprintID, myOtherVarName, UEdGraphSchema_K2::PC_Int, "10")};
    if (!MyOtherVarSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add MyOtherVar to blueprint!"));
        return;
    }

    auto MyResSuccess{AddVariable(blueprintID, myResultName, UEdGraphSchema_K2::PC_Int)};
    if (!MyResSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add MyRes to blueprint!"));
        return;
    }

    UpdateBlueprint(blueprintID);

    auto MyVarNode{AddVariableGetNode(blueprintID, myVarName, myVarNodeID)};
    auto MyOtherVarNode{AddVariableGetNode(blueprintID, myOtherVarName, myOtherVarNodeID)};

    auto AdditionNode{AddFunctionNode(blueprintID, functionName, functionNodeID)};
    if (!AdditionNode)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add AddFunctionNode"));
        return;
    }

    auto SetMyResultNode{AddVariableSetNode(blueprintID, myResultName, myResultNodeID)};
    if (!SetMyResultNode)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add SetMyResultNode"));
        return;
    }

    UpdateBlueprint(blueprintID);

    // 4. Connect all the nodes together

    // Connect the MyVariable getter to the addition node
    auto MyVarToAddSuccess{ConnectPins(myVarNodeID, myVarName, functionNodeID, "A")};
    if (!MyVarToAddSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect MyVariable to addition node"));
        return;
    }

    auto MyOtherVarToAddSuccess{ConnectPins(myOtherVarNodeID, myOtherVarName, functionNodeID, "B")};
    if (!MyOtherVarToAddSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect MyOtherVariable to addition node"));
        return;
    }

    // Connect the addition node to the MyResult setter
    auto AdditionToSetResultSuccess{ConnectPins(functionNodeID, "ReturnValue", myResultNodeID, myResultName)};
    if (!AdditionToSetResultSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect addition result to MyResult setter"));
        return;
    }

    // Optionally, if you want to organize the nodes visually, adjust their positions
    PositionNode(myVarNodeID, 0, 0);
    PositionNode(myOtherVarNodeID, 0, 200);
    PositionNode(functionNodeID, 200, 100);
    PositionNode(myResultNodeID, 400, 100);

    // Compile the blueprint after modifying

    CreateBlueprintPermanently(blueprintID);

    UE_LOG(LogTemp, Log, TEXT("Blueprint created and modified!"));
}
