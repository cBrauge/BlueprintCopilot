#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

namespace LibBlueprintCopilot::Guidance
{
    class AddVariable : public BaseAction
    {
    public:
        std::string BlueprintID;

        std::string VariableName;
        std::string PinCategory;
        std::string DefaultValue = "";
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddVariable, BlueprintID, VariableName, PinCategory, DefaultValue);

    inline std::string PrettyPrint(const AddVariable& addVariable)
    {
        std::ostringstream ss{};
        ss << "Add variable: " << addVariable.VariableName << " of type: " << addVariable.PinCategory
           << " with default value: '" << addVariable.DefaultValue << "' to Blueprint: " << addVariable.BlueprintID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance