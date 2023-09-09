#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

namespace LibBlueprintCopilot::Guidance
{
    class CreateBlueprint : public BaseAction
    {
    public:
        std::string BlueprintName;
        std::string BlueprintID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateBlueprint, BlueprintName, BlueprintID);

    inline std::string PrettyPrint(const CreateBlueprint& createBlueprint)
    {
        std::ostringstream ss{};
        ss << "Create Blueprint Name: " << createBlueprint.BlueprintName << " with ID: " << createBlueprint.BlueprintID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance