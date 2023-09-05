#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Actions/BaseAction.h"

namespace LibBlueprintCopilot::Guidance {
	class AddFunctionNode : public BaseAction {
	public:
		std::string BlueprintID;

		std::string FunctionName;
		std::string NodeID;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddFunctionNode, BlueprintID, FunctionName, NodeID);
}