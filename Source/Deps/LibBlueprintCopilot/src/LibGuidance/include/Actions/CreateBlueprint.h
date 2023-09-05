#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class CreateBlueprint : public BaseAction
    {
    public:
        std::string BlueprintName;
        std::string BlueprintID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateBlueprint, BlueprintName, BlueprintID);
} // namespace LibBlueprintCopilot::Guidance