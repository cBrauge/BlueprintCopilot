#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

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

    inline std::string PrettyPrint(const AddFunctionNode& addFunctionNode)
    {
        std::ostringstream ss{};
        ss << "Add function node: " << addFunctionNode.FunctionName
                  << " which will be nodeID: " << addFunctionNode.NodeID
                  << " inside BlueprintID: " << addFunctionNode.BlueprintID;
        
        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance