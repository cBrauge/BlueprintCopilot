#pragma once

#include <string>

namespace LibBlueprintCopilot::Guidance {
    class ILibLLM {
    public:
        virtual ~ILibLLM() = default;

        /// <summary>
        /// Sends a request to the llm and responds with the response from the llm
        /// </summary>
        virtual std::string Request(std::string_view request) = 0;
    };
}