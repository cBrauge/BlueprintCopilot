#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class AddTextBlockToWidgetBlueprint : public BaseAction
    {
    public:
        std::string BlueprintID;

        std::string Text;
        std::string NodeID;
        std::string Name;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddTextBlockToWidgetBlueprint, BlueprintID, Text, NodeID, Name);

    inline std::string PrettyPrint(const AddTextBlockToWidgetBlueprint& action)
    {
        std::ostringstream ss{};
        ss << "Add Text block to blueprint: " << action.BlueprintID << " with text: " << action.Text
           << " and Name: " << action.Name << " and will be referenceable with NodeID: " << action.NodeID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance