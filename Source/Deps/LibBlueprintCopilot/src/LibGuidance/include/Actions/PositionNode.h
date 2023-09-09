#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

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

    inline std::string PrettyPrint(const PositionNode& positionNode)
    {
        std::ostringstream ss{};
        ss << "Position nodeID: " << positionNode.NodeID << " in x:y: " << positionNode.x << ":" << positionNode.y;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance