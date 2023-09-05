#pragma once

#include <variant>

#include "Actions/CreateBlueprint.h"
#include "Actions/CreateLink.h"
#include "Actions/AddFunctionNode.h"
#include "Actions/AddVariable.h"
#include "Actions/AddVariableGetNode.h"
#include "Actions/AddVariableSetNode.h"
#include "Actions/CreateBlueprintPermanently.h"
#include "Actions/PositionNode.h"
#include "Actions/UpdateBlueprint.h"

namespace LibBlueprintCopilot::Guidance {
	using Action = std::variant<
		CreateBlueprint,
		CreateLink,
		AddFunctionNode,
		AddVariable,
		AddVariableGetNode,
		AddVariableSetNode,
		CreateBlueprintPermanently,
		PositionNode,
		UpdateBlueprint
	>;
}