#include "OpenAILibLLM.h"

namespace LibBlueprintCopilot::Guidance
{
    OpenAILibLLM::OpenAILibLLM(std::string_view api_key) : _api_key{api_key} {}

    std::string OpenAILibLLM::Request(std::string_view request)
    {
        return "OpenAI";
    }
} // namespace LibBlueprintCopilot::Guidance