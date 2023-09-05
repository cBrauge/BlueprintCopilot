#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Actions/BaseAction.h"

namespace LibBlueprintCopilot::Guidance {
	class CreateBlueprintPermanently : public BaseAction {
	public:
		std::string BlueprintID;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateBlueprintPermanently, BlueprintID);
}