#pragma once

#include "ILibLLM.h"

#include <memory>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    /// <summary>
    /// This class makes calls to OpenAI API
    /// </summary>
    class OpenAILibLLM : public ILibLLM
    {
    public:
        OpenAILibLLM(std::string_view api_key);
        OpenAILibLLM(OpenAILibLLM&& other) noexcept;
        OpenAILibLLM& operator=(OpenAILibLLM&& other) noexcept;

        ~OpenAILibLLM();

        /// <inheritdoc/>
        std::string Request(std::string_view request, std::string_view model) override;

    private:
        std::string _api_key;

    private:
        class Impl;
        std::unique_ptr<Impl> _pimpl;
    };
} // namespace LibBlueprintCopilot::Guidance
