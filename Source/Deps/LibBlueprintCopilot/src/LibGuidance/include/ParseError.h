#pragma once

#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class ParseError
    {
    public:
        ParseError(std::string reason) : Reason {reason}{}
        const std::string Reason;
    };

} // namespace LibBlueprintCopilot::Guidance