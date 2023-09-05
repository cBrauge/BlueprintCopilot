#pragma once

#include "ILibLLM.h"
#include "LibLLMModel.h"

#include <memory>
#include <string>

namespace LibBlueprintCopilot::Guidance
{
    class LibLLMFactory
    {
    public:
        static std::unique_ptr<ILibLLM> CreateLibLLM(LibLLMModel model, std::string_view api_key);
    };
} // namespace LibBlueprintCopilot::Guidance