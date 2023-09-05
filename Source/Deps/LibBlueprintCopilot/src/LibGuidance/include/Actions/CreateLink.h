#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Actions/BaseAction.h"

namespace LibBlueprintCopilot::Guidance {
	class CreateLink : public BaseAction {
	public:
		std::string SourceNodeID;
		std::string SourcePinName;

		std::string DestinationNodeID;
		std::string DestinationPinName;
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CreateLink, SourceNodeID, SourcePinName, DestinationNodeID, DestinationPinName);
}