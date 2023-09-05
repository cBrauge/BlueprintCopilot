#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Actions/BaseAction.h"

namespace LibBlueprintCopilot::Guidance {
	class CreateBlueprint : public BaseAction {
	public:
		std::string BlueprintName;
		std::string BlueprintID;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateBlueprint, BlueprintName, BlueprintID);
}