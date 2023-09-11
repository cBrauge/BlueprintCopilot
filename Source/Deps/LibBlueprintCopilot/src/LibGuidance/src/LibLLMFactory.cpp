#include "LibLLMFactory.h"

#include "AzureLibLLM.h"
#include "FakeLibLLM.h"
#include "OpenAILibLLM.h"

namespace LibBlueprintCopilot::Guidance
{
    std::unique_ptr<ILibLLM> LibLLMFactory::CreateLibLLM(LibLLMModel model, std::string_view api_key,
        std::optional<std::string_view> azure_resource_name, std::optional<std::string_view> azure_deployment_id,
        std::optional<std::string_view> azure_api_version)
    {
        if (model == LibLLMModel::FakeLLMModel)
        {
            return std::make_unique<FakeLibLLM>(FakeLibLLM());
        }
        else if (model == LibLLMModel::OpenAILLMModel)
        {
            return std::make_unique<OpenAILibLLM>(OpenAILibLLM(api_key));
        }
        else if (model == LibLLMModel::AzureLLMModel)
        {
            return std::make_unique<AzureLibLLM>(
                AzureLibLLM(api_key, azure_resource_name, azure_deployment_id, azure_api_version));
        }
    }
} // namespace LibBlueprintCopilot::Guidance