#pragma once

#include "Actions/Action.h"

#include <string>
#include <vector>

namespace LibBlueprintCopilot::Guidance
{
    std::vector<Action> ConvertResponseToActions(std::string_view response);
}