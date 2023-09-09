#pragma once

#include "Actions/BaseAction.h"

#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

namespace LibBlueprintCopilot::Guidance
{
    class CreateLink : public BaseAction
    {
    public:
        std::string SourceNodeID;
        std::string SourcePinName;

        std::string DestinationNodeID;
        std::string DestinationPinName;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateLink, SourceNodeID, SourcePinName, DestinationNodeID, DestinationPinName);

    inline std::string PrettyPrint(const CreateLink& createLink)
    {
        std::ostringstream ss{};
        ss << "Link from " << createLink.SourceNodeID << "." << createLink.SourcePinName
           << " to " << createLink.DestinationNodeID << "." << createLink.DestinationPinName;

        return ss.str();
    }
} // namespace LibBlueprintCopilot::Guidance