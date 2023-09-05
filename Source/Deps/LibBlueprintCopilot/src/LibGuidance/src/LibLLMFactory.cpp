#include "LibLLMFactory.h"

#include "FakeLibLLM.h"
#include "OpenAILibLLM.h"

namespace LibBlueprintCopilot::Guidance
{
    std::unique_ptr<ILibLLM> LibLLMFactory::CreateLibLLM(LibLLMModel model, std::string_view api_key)
    {
        if (model == LibLLMModel::FakeLLMModel)
        {
            return std::make_unique<FakeLibLLM>(FakeLibLLM());
        }
        else if (model == LibLLMModel::OpenAILLMModel)
        {
            return std::make_unique<OpenAILibLLM>(OpenAILibLLM(api_key));
        }
    }
} // namespace LibBlueprintCopilot::Guidance