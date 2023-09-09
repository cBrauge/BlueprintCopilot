#pragma once

#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class ILibLLM
    {
    public:
        virtual ~ILibLLM() = default;

        /// <summary>
        /// Sends a request to the llm, responds synchronously (can take some time)
        /// </summary>
        /// <param name="request">The input text from the user</param>
        /// <param name="model">Which model to use: gpt-4, gpt-3.5-turbo</param>
        /// <returns>Instructions to execute</returns>
        virtual std::string Request(std::string_view request, std::string_view model) = 0;
    };
} // namespace LibBlueprintCopilot::Guidance