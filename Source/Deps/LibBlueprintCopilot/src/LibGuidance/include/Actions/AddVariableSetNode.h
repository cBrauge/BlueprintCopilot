#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class AddVariableSetNode : public BaseAction
    {
    public:
        std::string BlueprintID;

        std::string PropertyName;
        std::string NodeID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddVariableSetNode, BlueprintID, PropertyName, NodeID);
} // namespace LibBlueprintCopilot::Guidance