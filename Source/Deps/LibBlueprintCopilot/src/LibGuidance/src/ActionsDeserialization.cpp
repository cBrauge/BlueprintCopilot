#include "ActionsDeserialization.h"

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace LibBlueprintCopilot::Guidance
{
    std::vector<Action> ConvertResponseToActions(std::string_view response)
    {
        auto json{nlohmann::json::parse(response)};

        std::vector<Action> actions{};
        for (const auto& item : json)
        {
            if (!item.contains("Type"))
            {
                std::cout << "No Type for object" << std::endl;
                continue;
            }

            const auto& type{item["Type"]};

            if (type == "CreateBlueprint")
            {
                auto createBlueprint{item.get<CreateBlueprint>()};
                std::cout << "Create Blueprint Name: " << createBlueprint.BlueprintName
                          << " with ID: " << createBlueprint.BlueprintID << std::endl;
                actions.emplace_back(createBlueprint);
            }
            else if (type == "CreateLink")
            {
                auto createLink{item.get<CreateLink>()};
                std::cout << "Link from " << createLink.SourceNodeID << "." << createLink.SourcePinName;
                std::cout << " to " << createLink.DestinationNodeID << "." << createLink.DestinationPinName
                          << std::endl;
                actions.emplace_back(createLink);
            }
            else if (type == "AddVariable")
            {
                auto addVariable{item.get<AddVariable>()};
                std::cout << "Add variable: " << addVariable.VariableName << " of type: " << addVariable.PinCategory
                          << " with default value: '" << addVariable.DefaultValue
                          << "' to Blueprint: " << addVariable.BlueprintID << std::endl;
                actions.emplace_back(addVariable);
            }
            else if (type == "AddVariableGetNode")
            {
                auto addVariableGetNode{item.get<AddVariableGetNode>()};
                std::cout << "Add variable getter node: " << addVariableGetNode.PropertyName
                          << " which will be nodeID: " << addVariableGetNode.NodeID
                          << " inside BlueprintID: " << addVariableGetNode.BlueprintID << std::endl;
                actions.emplace_back(addVariableGetNode);
            }
            else if (type == "AddVariableSetNode")
            {
                auto addVariableSetNode{item.get<AddVariableSetNode>()};
                std::cout << "Add variable setter node: " << addVariableSetNode.PropertyName
                          << " which will be nodeID: " << addVariableSetNode.NodeID
                          << " inside BlueprintID: " << addVariableSetNode.BlueprintID << std::endl;
                actions.emplace_back(addVariableSetNode);
            }
            else if (type == "AddFunctionNode")
            {
                auto addFunctionNode{item.get<AddFunctionNode>()};
                std::cout << "Add function node: " << addFunctionNode.FunctionName
                          << " which will be nodeID: " << addFunctionNode.NodeID
                          << " inside BlueprintID: " << addFunctionNode.BlueprintID << std::endl;
                actions.emplace_back(addFunctionNode);
            }
            else if (type == "PositionNode")
            {
                auto positionNode{item.get<PositionNode>()};
                std::cout << "Position nodeID: " << positionNode.NodeID << " in x:y: " << positionNode.x << ":"
                          << positionNode.y << std::endl;
                actions.emplace_back(positionNode);
            }
            else if (type == "UpdateBlueprint")
            {
                auto updateBlueprint{item.get<UpdateBlueprint>()};
                std::cout << "Update blueprintID: " << updateBlueprint.BlueprintID << std::endl;
                actions.emplace_back(updateBlueprint);
            }
            else if (type == "CreateBlueprintPermanently")
            {
                auto createBlueprintPermanently{item.get<CreateBlueprintPermanently>()};
                std::cout << "Create blueprintID on disk: " << createBlueprintPermanently.BlueprintID << std::endl;
                actions.emplace_back(createBlueprintPermanently);
            }
            else
            {
                std::cout << "Unknown type: " << type << std::endl;
            }
        }

        return actions;
    }
} // namespace LibBlueprintCopilot::Guidance