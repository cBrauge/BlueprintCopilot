#pragma once

#include "Actions/AddFunctionNode.h"
#include "Actions/AddTextBlockToWidgetBlueprint.h"
#include "Actions/AddVariable.h"
#include "Actions/AddVariableGetNode.h"
#include "Actions/AddVariableSetNode.h"
#include "Actions/AssignNode.h"
#include "Actions/CreateBlueprint.h"
#include "Actions/CreateBlueprintPermanently.h"
#include "Actions/CreateLink.h"
#include "Actions/ManualOperation.h"
#include "Actions/PositionNode.h"
#include "Actions/SetPinValue.h"
#include "Actions/UpdateBlueprint.h"

#include <variant>

namespace LibBlueprintCopilot::Guidance
{
    using Action = std::variant<AssignNode, AddTextBlockToWidgetBlueprint, CreateBlueprint, CreateLink, AddFunctionNode,
        AddVariable, AddVariableGetNode, AddVariableSetNode, CreateBlueprintPermanently, PositionNode, UpdateBlueprint,
        SetPinValue, ManualOperation>;
}