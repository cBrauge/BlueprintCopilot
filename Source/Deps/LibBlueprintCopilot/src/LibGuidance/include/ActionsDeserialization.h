#include <vector>
#include <string>

#include "Actions/Action.h"

namespace LibBlueprintCopilot::Guidance {
	std::vector<Action> ConvertResponseToActions(std::string_view response);
}