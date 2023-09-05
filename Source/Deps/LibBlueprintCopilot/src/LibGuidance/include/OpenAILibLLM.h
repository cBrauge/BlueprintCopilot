#pragma once

#include "ILibLLM.h"

#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class OpenAILibLLM : public ILibLLM
    {
    public:
        OpenAILibLLM(std::string_view api_key);

        std::string Request(std::string_view request) override;

    private:
        std::string _api_key;
    };
} // namespace LibBlueprintCopilot::Guidance
