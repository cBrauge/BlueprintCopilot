#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Actions/BaseAction.h"

namespace LibBlueprintCopilot::Guidance {
	class UpdateBlueprint : public BaseAction {
	public:
		std::string BlueprintID;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UpdateBlueprint, BlueprintID);
}