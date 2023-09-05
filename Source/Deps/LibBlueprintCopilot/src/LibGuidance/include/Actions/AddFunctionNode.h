#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class AddFunctionNode : public BaseAction
    {
    public:
        std::string BlueprintID;

        std::string FunctionName;
        std::string NodeID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddFunctionNode, BlueprintID, FunctionName, NodeID);
} // namespace LibBlueprintCopilot::Guidance