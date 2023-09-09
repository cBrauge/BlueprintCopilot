#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

namespace LibBlueprintCopilot::Guidance
{
    class UpdateBlueprint : public BaseAction
    {
    public:
        std::string BlueprintID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UpdateBlueprint, BlueprintID);

    inline std::string PrettyPrint(const UpdateBlueprint& updateBlueprint)
    {
        std::ostringstream ss{};
        ss << "Update blueprintID: " << updateBlueprint.BlueprintID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance