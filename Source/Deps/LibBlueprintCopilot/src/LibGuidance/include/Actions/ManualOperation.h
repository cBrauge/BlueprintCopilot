#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class ManualOperation : public BaseAction
    {
    public:
        std::string WhatToDo;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ManualOperation, WhatToDo);

    inline std::string PrettyPrint(const ManualOperation& manualOperation)
    {
        std::ostringstream ss{};
        ss << "Manual operation: " << manualOperation.WhatToDo;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance