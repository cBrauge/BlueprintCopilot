#include "ActionsDeserialization.h"

#include <iostream>
#include <unordered_map>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace LibBlueprintCopilot::Guidance
{
    std::vector<Action> ConvertResponseToActions(std::string_view response)
    {
        const auto json{nlohmann::json::parse(response)};

        std::vector<Action> actions{};
        for (const auto& item : json)
        {
            if (!item.contains("Type"))
            {
                std::cerr << "No Type for object" << std::endl;
                continue;
            }

            const auto& type{item["Type"]};
            if (type == "CreateBlueprint")
            {
                actions.emplace_back(item.get<CreateBlueprint>());
            }
            else if (type == "CreateLink")
            {
                actions.emplace_back(item.get<CreateLink>());
            }
            else if (type == "AddVariable")
            {
                actions.emplace_back(item.get<AddVariable>());
            }
            else if (type == "AddVariableGetNode")
            {
                actions.emplace_back(item.get<AddVariableGetNode>());
            }
            else if (type == "AddVariableSetNode")
            {
                actions.emplace_back(item.get<AddVariableSetNode>());
            }
            else if (type == "AddFunctionNode")
            {
                actions.emplace_back(item.get<AddFunctionNode>());
            }
            else if (type == "PositionNode")
            {
                actions.emplace_back(item.get<PositionNode>());
            }
            else if (type == "UpdateBlueprint")
            {
                actions.emplace_back(item.get<UpdateBlueprint>());
            }
            else if (type == "CreateBlueprintPermanently")
            {
                actions.emplace_back(item.get<CreateBlueprintPermanently>());
            }
            else
            {
                std::cerr << "Unknown type: " << type << std::endl;
            }
        }

        return actions;
    }
} // namespace LibBlueprintCopilot::Guidance