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
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddTextBlockToWidgetBlueprint, BlueprintID, Text);

    inline std::string PrettyPrint(const AddTextBlockToWidgetBlueprint& action)
    {
        std::ostringstream ss{};
        ss << "Add Text block to blueprint: " << action.BlueprintID << " with text: " << action.Text;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance