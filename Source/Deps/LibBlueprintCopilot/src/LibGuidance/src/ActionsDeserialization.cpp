#include "ActionsDeserialization.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>

using json = nlohmann::json;

namespace LibBlueprintCopilot::Guidance
{
    std::variant<json, ParseError> GetParsedData(std::string_view response)
    {
        size_t start = response.find('[');
        size_t end   = response.rfind(']');

        if (!(start != std::string::npos && end != std::string::npos && start < end))
        {
            nlohmann::json error_obj = {{"error", "Does not contain a json array"}, {"full_input", response}};
            return ParseError(error_obj.dump());
        }

        auto array_str{response.substr(start, end - start + 1)};

        try
        {
            return nlohmann::json::parse(array_str);
        }
        catch (nlohmann::json::parse_error& e)
        {
            nlohmann::json error_obj = {{"error", e.what()}, {"position", e.byte}, {"full_input", array_str}};

            return ParseError(error_obj.dump());
        }
    }

    std::variant<std::vector<Action>, ParseError> ConvertResponseToActions(std::string_view response)
    {
        auto json{GetParsedData(response)};
        if (std::holds_alternative<ParseError>(json))
        {
            return std::get<ParseError>(json);
        }

        std::vector<Action> actions{};
        for (const auto& item : std::get<nlohmann::json>(json))
        {
            if (!item.contains("Type"))
            {
                nlohmann::json error_obj = {{"error", "No 'type' for object"}, {"full_input", item.dump()}};
                return ParseError(error_obj.dump());
            }

            try
            {
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
                    const auto& x{item["x"]};
                    const auto& y{item["y"]};
                    // It can happen that the llm gives us back string instead of numbers
                    if (x.is_number() && y.is_number())
                    {
                        actions.emplace_back(item.get<PositionNode>());
                    }
                    else
                    {
                        auto x_value = 0;
                        auto y_value = 0;
                        if (!x.is_number())
                        {
                            x_value = std::stoi(x.get<std::string>());
                        }
                        else
                        {
                            x_value = x.get<int>();
                        }

                        if (!y.is_number())
                        {
                            y_value = std::stoi(y.get<std::string>());
                        }
                        else
                        {
                            y_value = y.get<int>();
                        }

                        PositionNode p{};
                        p.NodeID = item["NodeID"].get<std::string>();
                        p.x      = x_value;
                        p.y      = y_value;
                        actions.emplace_back(p);
                    }
                }
                else if (type == "UpdateBlueprint")
                {
                    actions.emplace_back(item.get<UpdateBlueprint>());
                }
                else if (type == "CreateBlueprintPermanently")
                {
                    actions.emplace_back(item.get<CreateBlueprintPermanently>());
                }
                else if (type == "ManualOperation")
                {
                    actions.emplace_back(item.get<ManualOperation>());
                }
                else if (type == "AddTextBlockToWidgetBlueprint")
                {
                    actions.emplace_back(item.get<AddTextBlockToWidgetBlueprint>());
                }
                else if (type == "AssignNode")
                {
                    actions.emplace_back(item.get<AssignNode>());
                }
                else if (type == "SetPinValue")
                {
                    actions.emplace_back(item.get<SetPinValue>());
                }
                else
                {
                    nlohmann::json error_obj = {{"error", "Unknown type"}, {"full_input", item.dump()}};
                    return ParseError(error_obj.dump());
                }
            }
            catch (nlohmann::json::parse_error& e)
            {
                nlohmann::json error_obj = {{"error", e.what()}, {"position", e.byte}, {"full_input", item}};
                return ParseError(error_obj.dump());
            }
        }

        return actions;
    }
} // namespace LibBlueprintCopilot::Guidance