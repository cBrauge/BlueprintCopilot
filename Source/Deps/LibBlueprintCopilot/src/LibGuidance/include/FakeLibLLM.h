#pragma once

#include "ILibLLM.h"

#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class FakeLibLLM : public ILibLLM
    {
    public:
        std::string Request(std::string_view request) override;
    };
} // namespace LibBlueprintCopilot::Guidance