#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class PositionNode : public BaseAction
    {
    public:
        std::string NodeID;

        int x;
        int y;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PositionNode, NodeID, x, y);
} // namespace LibBlueprintCopilot::Guidance