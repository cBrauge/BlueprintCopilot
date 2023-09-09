#pragma once

#include "ILibLLM.h"

#include <string>

namespace LibBlueprintCopilot::Guidance
{
    /// <summary>
    /// This class is not making any calls to a service, it simply reply with the input.
    /// This is useful if you're debugging a certain reply from the llm and don't want to use
    /// credits
    /// </summary>
    class FakeLibLLM : public ILibLLM
    {
    public:
        /// <inheritdoc/>
        std::string Request(std::string_view request, std::string_view model) override;
    };
} // namespace LibBlueprintCopilot::Guidance