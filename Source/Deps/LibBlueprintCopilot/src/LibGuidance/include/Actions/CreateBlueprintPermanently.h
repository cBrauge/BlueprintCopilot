#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class CreateBlueprintPermanently : public BaseAction
    {
    public:
        std::string BlueprintID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateBlueprintPermanently, BlueprintID);
} // namespace LibBlueprintCopilot::Guidance