#include "FakeLibLLM.h"

namespace LibBlueprintCopilot::Guidance
{
    std::string FakeLibLLM::Request(std::string_view request)
    {
        return std::string{request};
    }
} // namespace LibBlueprintCopilot::Guidance