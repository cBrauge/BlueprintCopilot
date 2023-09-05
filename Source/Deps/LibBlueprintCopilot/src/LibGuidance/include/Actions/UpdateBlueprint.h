#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class UpdateBlueprint : public BaseAction
    {
    public:
        std::string BlueprintID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UpdateBlueprint, BlueprintID);
} // namespace LibBlueprintCopilot::Guidance