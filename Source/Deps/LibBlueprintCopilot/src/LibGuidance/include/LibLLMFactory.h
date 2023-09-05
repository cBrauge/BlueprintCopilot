#pragma once

#include <memory>
#include <string>

#include "ILibLLM.h"
#include "LibLLMModel.h"

namespace LibBlueprintCopilot::Guidance {
	class LibLLMFactory {
	public:
		static std::unique_ptr<ILibLLM> CreateLibLLM(LibLLMModel model, std::string_view api_key);
	};
}