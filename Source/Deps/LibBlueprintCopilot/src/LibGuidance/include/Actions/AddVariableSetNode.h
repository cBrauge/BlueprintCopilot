#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

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

    inline std::string PrettyPrint(const AddVariableSetNode& addVariableSetNode)
    {
        std::ostringstream ss{};
        ss << "Add variable setter node: " << addVariableSetNode.PropertyName
           << " which will be nodeID: " << addVariableSetNode.NodeID
           << " inside BlueprintID: " << addVariableSetNode.BlueprintID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance