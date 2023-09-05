#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Actions/BaseAction.h"

namespace LibBlueprintCopilot::Guidance {
	class AddVariableGetNode : public BaseAction {
	public:
		std::string BlueprintID;

		std::string PropertyName;
		std::string NodeID;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddVariableGetNode, BlueprintID, PropertyName, NodeID);
}