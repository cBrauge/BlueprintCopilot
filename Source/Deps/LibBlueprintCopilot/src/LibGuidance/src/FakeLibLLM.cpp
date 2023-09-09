#include "FakeLibLLM.h"

namespace LibBlueprintCopilot::Guidance
{
    std::string FakeLibLLM::Request(std::string_view request, std::string_view model)
    {
        return std::string{request};
    }
} // namespace LibBlueprintCopilot::Guidance