#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Actions/BaseAction.h"

namespace LibBlueprintCopilot::Guidance {
	class PositionNode : public BaseAction {
	public:
		std::string NodeID;

		int x;
		int y;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PositionNode, NodeID, x, y);
}