#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

namespace LibBlueprintCopilot::Guidance
{
    class AddVariableGetNode : public BaseAction
    {
    public:
        std::string BlueprintID;

        std::string PropertyName;
        std::string NodeID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddVariableGetNode, BlueprintID, PropertyName, NodeID);

    inline std::string PrettyPrint(const AddVariableGetNode& addVariableGetNode)
    {
        std::ostringstream ss{};
        ss << "Add variable getter node: " << addVariableGetNode.PropertyName
           << " which will be nodeID: " << addVariableGetNode.NodeID
           << " inside BlueprintID: " << addVariableGetNode.BlueprintID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance