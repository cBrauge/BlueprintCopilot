#pragma once

#include "ILibLLM.h"

#include <memory>
#include <optional>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    /// <summary>
    /// This class makes calls to Azure API
    /// </summary>
    class AzureLibLLM : public ILibLLM
    {
    public:
        AzureLibLLM(std::string_view api_key, std::optional<std::string_view> azure_resource_name,
            std::optional<std::string_view> azure_deployment_id, std::optional<std::string_view> azure_api_version);
        AzureLibLLM(AzureLibLLM&& other) noexcept;
        AzureLibLLM& operator=(AzureLibLLM&& other) noexcept;

        ~AzureLibLLM();

        /// <inheritdoc/>
        std::string Request(std::string_view request, std::string_view model) override;

    private:
        std::string _api_key;

    private:
        class Impl;
        std::unique_ptr<Impl> _pimpl;
    };
} // namespace LibBlueprintCopilot::Guidance
