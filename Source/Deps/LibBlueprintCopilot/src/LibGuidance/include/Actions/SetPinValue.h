#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class SetPinValue : public BaseAction
    {
    public:
        std::string NodeID;

        std::string PinName;
        std::string Value;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SetPinValue, NodeID, PinName, Value);

    inline std::string PrettyPrint(const SetPinValue& action)
    {
        std::ostringstream ss{};
        ss << "Set Pin Value on node: " << action.NodeID << " with value: " << action.Value
           << " for PinName: " << action.PinName;
        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance