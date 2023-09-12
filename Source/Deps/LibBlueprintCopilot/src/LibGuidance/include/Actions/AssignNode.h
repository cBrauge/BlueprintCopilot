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
        std::string BlueprintID;
        std::string NodeID;
        std::string NodeName;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AssignNode, BlueprintID, NodeID, NodeName);

    inline std::string PrettyPrint(const AssignNode& action)
    {
        std::ostringstream ss{};
        ss << "Assign: " << action.NodeName << " to ID: " << action.NodeID << " in Blueprint: " << action.BlueprintID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance