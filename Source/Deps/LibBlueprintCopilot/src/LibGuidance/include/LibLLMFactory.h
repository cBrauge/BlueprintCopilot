#pragma once

#include "ILibLLM.h"
#include "LibLLMModel.h"

#include <memory>
#include <optional>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class LibLLMFactory
    {
    public:
        /// <summary>
        /// Based on the model, it returns an interface to query the llm
        /// </summary>
        /// <param name="api_key">Your API key for the service</param>
        static std::unique_ptr<ILibLLM> CreateLibLLM(LibLLMModel model, std::string_view api_key,
            std::optional<std::string_view> azure_resource_name = std::nullopt,
            std::optional<std::string_view> azure_deployment_id = std::nullopt,
            std::optional<std::string_view> azure_api_version   = std::nullopt);
    };
} // namespace LibBlueprintCopilot::Guidance