#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class AssignNode : public BaseAction
    {
    public:
        std::string NodeID;
        std::string NodeName;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AssignNode, NodeID, NodeName);

    inline std::string PrettyPrint(const AssignNode& action)
    {
        std::ostringstream ss{};
        ss << "Assign: " << action.NodeName << " to ID: " << action.NodeID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance