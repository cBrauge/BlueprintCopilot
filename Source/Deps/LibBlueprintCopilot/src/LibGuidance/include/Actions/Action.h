#pragma once

#include "Actions/AddFunctionNode.h"
#include "Actions/AddVariable.h"
#include "Actions/AddVariableGetNode.h"
#include "Actions/AddVariableSetNode.h"
#include "Actions/CreateBlueprint.h"
#include "Actions/CreateBlueprintPermanently.h"
#include "Actions/CreateLink.h"
#include "Actions/PositionNode.h"
#include "Actions/UpdateBlueprint.h"
#include "Actions/ManualOperation.h"

#include <variant>

namespace LibBlueprintCopilot::Guidance
{
    using Action = std::variant<CreateBlueprint, CreateLink, AddFunctionNode, AddVariable, AddVariableGetNode,
        AddVariableSetNode, CreateBlueprintPermanently, PositionNode, UpdateBlueprint, ManualOperation>;
}