#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>

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
} // namespace LibBlueprintCopilot::Guidance