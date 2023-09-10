#pragma once

#include "Actions/Action.h"
#include "ParseError.h"

#include <string>
#include <vector>
#include <variant>

namespace LibBlueprintCopilot::Guidance
{
    std::variant<std::vector<Action>, ParseError> ConvertResponseToActions(std::string_view response);
}