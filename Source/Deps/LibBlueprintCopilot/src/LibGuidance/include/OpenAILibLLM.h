#pragma once

#include <string>

#include "ILibLLM.h"

namespace LibBlueprintCopilot::Guidance {
	class OpenAILibLLM : public ILibLLM {
	public:
		OpenAILibLLM(std::string_view api_key);

		std::string Request(std::string_view request) override;

	private:
		std::string _api_key;
	};
}
