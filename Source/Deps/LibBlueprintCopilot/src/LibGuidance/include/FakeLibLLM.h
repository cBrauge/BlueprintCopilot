#pragma once

#include <string>

#include "ILibLLM.h"

namespace LibBlueprintCopilot::Guidance {
	class FakeLibLLM : public ILibLLM {
	public:
		std::string Request(std::string_view request) override;

	};
}