#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class CreateBlueprint : public BaseAction
    {
    public:
        std::string BlueprintName;
        std::string BlueprintID;
        // TODO add verification that it needs to be either Actor or Widget
        std::string BlueprintType;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateBlueprint, BlueprintName, BlueprintID, BlueprintType);

    inline std::string PrettyPrint(const CreateBlueprint& createBlueprint)
    {
        std::ostringstream ss{};
        ss << "Create Blueprint Name: " << createBlueprint.BlueprintName << " with ID: " << createBlueprint.BlueprintID
           << " with Type: " << createBlueprint.BlueprintType;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance