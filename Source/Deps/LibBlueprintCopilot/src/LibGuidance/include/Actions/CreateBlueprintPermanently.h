#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

namespace LibBlueprintCopilot::Guidance
{
    class CreateBlueprintPermanently : public BaseAction
    {
    public:
        std::string BlueprintID;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateBlueprintPermanently, BlueprintID);

    inline std::string PrettyPrint(const CreateBlueprintPermanently& createBlueprintPermanently)
    {
        std::ostringstream ss{};
        ss << "Create blueprintID on disk: " << createBlueprintPermanently.BlueprintID;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance