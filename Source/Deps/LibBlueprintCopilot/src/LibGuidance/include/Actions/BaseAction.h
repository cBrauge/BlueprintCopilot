#pragma once

#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class BaseAction
    {
    public:
        std::string OriginalJson;
        std::string Type;
    };
} // namespace LibBlueprintCopilot::Guidance