#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Actions/BaseAction.h"

namespace LibBlueprintCopilot::Guidance {
	class AddVariable : public BaseAction {
	public:
		std::string BlueprintID;

		std::string VariableName;
		std::string PinCategory;
		std::string DefaultValue = "";
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddVariable, BlueprintID, VariableName, PinCategory, DefaultValue);
}